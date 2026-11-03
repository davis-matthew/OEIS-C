/*
 * A142322  Primes congruent to 19 mod 45.
 * https://oeis.org/A142322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 19 modulo 45
 * Range:     u8 n <= 3, u16 n <= 266, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 60 terms through n = 954 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>   /* for size_t */
#include <stdbool.h>
#include <stdint.h>
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
    /* Undefined for n == 0 – return 0 as a sentinel */
    if (n == 0)
        return (seq_t)0;

    /* Static cache of found primes */
    static seq_t *primes = NULL;
    static idx_t  count  = 0;     /* number of primes cached */
    static size_t cap    = 0;     /* allocated capacity in elements */
    static idx_t  candidate = 19; /* next integer ≡ 19 (mod 45) */

    /* Grow the cache until we have at least n primes */
    while (count < n)
    {
        /* Test the current candidate for primality */
        if (is_prime_big((seq_t)candidate))
        {
            /* Ensure enough space for the new prime */
            if (count == cap)
            {
                size_t new_cap = (cap == 0) ? 16 : cap * 2;
                seq_t *tmp = mem_resize(seq_t, primes, new_cap);
                if (!tmp)
                    abort();                     /* OOM */
                primes = tmp;
                cap    = new_cap;
            }
            /* Store the new prime */
            primes[count++] = (seq_t)candidate;
        }
        /* Advance to the next candidate with residue 19 modulo 45 */
        candidate += 45;
    }

    /* Return the (n-1)-th cached prime */
    return primes[n - 1];
}
