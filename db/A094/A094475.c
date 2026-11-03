/*
 * A094475  Primes of form 2^n + 5^n.
 * https://oeis.org/A094475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime of the form 2^k + 5^k
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: partial, pass, 4 terms through n = 4 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The routine searches increasing exponents k, computes 2^k + 5^k,
   tests each candidate for primality, and stores the primes in order.
   Results are memoised so calls may arrive in any order.  The code
   accepts the supplied types seq_t and idx_t, and relies on the
   helper functions isPrime (for 64‑bit integers) and
   is_prime_big (for arbitrary‑precision integers). */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
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

static inline bool is_prime_seq(seq_t value)
{
    /* Use the 64‑bit test when the value fits in idx_t, otherwise
       use the generic big‑int routine. */
    if (sizeof(seq_t) <= sizeof(idx_t))
    {
        return is_prime((idx_t)value);
    }
    else
    {
        return is_prime_big(value);
    }
}

/* Compute base^exp using repeated multiplication.
   Works for arbitrary seq_t types as long as multiplication and
   addition are defined. */
static seq_t pow_int_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Main sequence routine. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* out of range */

    /* Static storage for previously found primes. */
    static seq_t *primes = NULL;
    static size_t prime_len = 0, prime_cap = 0;

    /* Current values of 2^k and 5^k. */
    static seq_t pow2 = (seq_t)1, pow5 = (seq_t)1;
    static idx_t next_k = 0;          /* exponent already processed */

    /* Initialise on first call. */
    if (primes == NULL)
    {
        prime_cap = 16;
        primes = mem_new(seq_t, prime_cap);
        if (primes == NULL)
            return (seq_t)0;   /* allocation failed */
    }

    /* Generate primes until we have at least n of them. */
    while (prime_len < (size_t)n)
    {
        /* Candidate for exponent next_k: 2^next_k + 5^next_k. */
        seq_t candidate = pow2 + pow5;

        if (is_prime_seq(candidate))
        {
            if (prime_len == prime_cap)
            {
                prime_cap <<= 1;
                seq_t *tmp = mem_resize(seq_t, primes, prime_cap);
                if (tmp == NULL)
                    return (seq_t)0;   /* realloc failed */
                primes = tmp;
            }
            primes[prime_len++] = candidate;
        }

        /* Advance to the next exponent. */
        pow2 *= (seq_t)2;
        pow5 *= (seq_t)5;
        ++next_k;
    }

    return primes[n - 1];
}
