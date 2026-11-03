/*
 * A128878  Primes of the form 47*n^2 - 1701*n + 10181.
 * https://oeis.org/A128878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime of the polynomial 47*k^2 - 1701*k + 10181,
 *            enumerated by increasing integer k (starting from k = 0)
 * Range:     u8 none, u16 n <= 29, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Validated: partial, pass, 37 terms through n = 37 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
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
    /* Cache of primes found so far */
    static seq_t *prime_cache      = NULL;   /* array of primes */
    static size_t  prime_count    = 0;        /* number of primes stored */
    static size_t  prime_capacity = 0;        /* allocated capacity */

    /* Next integer k to test in the polynomial */
    static idx_t   next_k          = 0;

    /* Return a dummy value for n = 0 (out of offset) */
    if (n == 0)
        return 0;

    /* Grow cache until we have at least n primes */
    while (prime_count < n)
    {
        /* Compute polynomial value in type seq_t to avoid overflow */
        /* the polynomial is negative for 8 <= k <= 28, which wrapped around
           in an unsigned seq_t: evaluate it signed, and test only values > 1 */
        __int128 kk = (__int128)next_k;
        __int128 v128 = 47 * kk * kk - 1701 * kk + 10181;
        seq_t val = v128 > 1 ? (seq_t)(unsigned __int128)v128 : (seq_t)0;

        if (v128 > 1)
        {
            /* For small seq_t types, skip values that would overflow the type */
            if (sizeof(seq_t) <= sizeof(uint64_t))
            {
                seq_t max_seq_t = ~ (seq_t)0;
                if (val > max_seq_t)
                {
                    next_k++;
                    continue;       /* value too large for seq_t */
                }
            }

            if (is_prime_big(val))
            {
                /* Ensure there is space for the new prime */
                if (prime_count == prime_capacity)
                {
                    size_t new_cap = (prime_capacity == 0) ? 8 : (prime_capacity << 1);
                    seq_t *tmp = mem_resize(seq_t, prime_cache, new_cap);
                    prime_cache = tmp;
                    prime_capacity = new_cap;
                }
                prime_cache[prime_count++] = val;
            }
        }

        /* Advance k for next iteration */
        next_k++;
    }

    /* Return the requested prime, 1‑based index */
    return prime_cache[n - 1];
}
