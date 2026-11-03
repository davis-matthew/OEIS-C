/*
 * A167133  Primes of the form (number of prime factors of k-th composite)
 *          plus (number of divisors of k-th composite).
 * https://oeis.org/A167133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime equal to (number of prime factors with multiplicity
 *            of the n-th composite) + * (number of divisors of the n-th
 *            composite)
 * Range:     u8 n <= 46, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 90 terms through n = 9336 (2026-11-03)
 * Helpers:   bigomega_seq, is_prime, is_prime_u64, mem, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/bigomega_seq.h"
#include "../utils/is_prime.h"
#include "../utils/num_divisors.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* No sequence value for n = 0 (the sequence starts with offset 1). */
    if (n == 0)
        return 0;

    /* Static state to allow calls in arbitrary order. */
    static seq_t *prime_values = NULL;  /* array of found primes */
    static size_t prime_cap = 0;        /* capacity of prime_values      */
    static size_t prime_count = 0;      /* number of primes stored       */
    static uint64_t next_comp = 4;      /* next integer to test for being a composite */
    /* The following static variables are not strictly necessary, but they keep
       the code clearer. */
    static bool initialized = false;

    if (!initialized)
    {
        prime_cap = 128;
        prime_values = mem_new(seq_t, prime_cap);
        if (!prime_values)
            return 0; /* allocation failure – unlikely in test environment */
        initialized = true;
    }

    /* Generate composites and collect qualifying primes until we have at
       least n primes. */
    while (prime_count < n)
    {
        /* Find next composite number. */
        while (true)
        {
            if (next_comp <= 3) { /* 4 is the first composite */
                next_comp = 4;
                break;
            }
            if (!is_prime((idx_t)next_comp))
                break;
            ++next_comp;
        }

        /* Compute omega (number of prime factors with multiplicity) and
           divisor count d(n). */
        seq_t omega = bigomega_seq((seq_t)next_comp);
        uint64_t d = num_divisors(next_comp);
        seq_t sum = omega + (seq_t)d;  /* add d to omega; sum fits in seq_t */

        /* Check if the sum is prime; if so, store it. */
        if (is_prime_big(sum))
        {
            if (prime_count == prime_cap)
            {
                prime_cap <<= 1;
                seq_t *tmp = mem_resize(seq_t, prime_values, prime_cap);
                if (!tmp)
                    return 0; /* allocation failure */
                prime_values = tmp;
            }
            prime_values[prime_count++] = sum;
        }

        /* Advance to the next integer for the next composite search. */
        ++next_comp;
    }

    /* We now have at least n primes stored; return the n-th (1‑based). */
    return prime_values[n - 1];
}
