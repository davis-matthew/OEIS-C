/*
 * A216590  Primes p with property that there exists a number d>0 such that
 *          numbers p-k*d, k=1...7, are seven primes.
 * https://oeis.org/A216590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   computed via search over primes p such that there exists a
 *            positive integer d with 210 | d for which the seven numbers p −
 *            k*d (k = 1 … 7) are all primes
 * Range:     u8 none, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-03)
 * Helpers:   is_prime_u64, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/next_prime.h"
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
    if (n == 0) return 0;  /* safety for calls with n < 1 */

    /* cached results of the sequence */
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;

    /* last prime used as a starting point for next search */
    static idx_t last_checked = 1;  /* prime 2 will be found next */

    /* generate terms up to the requested index */
    while (terms_len < (size_t)n)
    {
        /* get next prime */
        last_checked = next_prime(last_checked);

        /* d must be a multiple of 210; try all such d until
           p - 7*d < 2.  The maximum m is floor((p-2)/1470). */
        idx_t p = last_checked;
        idx_t m_max = (p > 2) ? (p - 2) / 1470 : 0;
        bool found = false;

        for (idx_t m = 1; m <= m_max && !found; ++m)
        {
            seq_t d = (seq_t)210 * (seq_t)m;
            bool ok = true;

            for (int k = 1; k <= 7; ++k)
            {
                seq_t val = (seq_t)p - (seq_t)k * d;
                if (!is_prime_big(val))
                {
                    ok = false;
                    break;
                }
            }

            if (ok)
            {
                /* store the found prime */
                if (terms_len == terms_cap)
                {
                    terms_cap = terms_cap ? terms_cap * 2 : 16;
                    terms = mem_resize(seq_t, terms, terms_cap);
                    if (!terms) return 0;   /* allocation failure */
                }
                terms[terms_len++] = p;
                found = true;
            }
        }
        /* If no suitable d was found for this prime, simply go to the
           next prime.  The loop continues until the array contains
           enough terms. */
    }

    return terms[n - 1];
}
