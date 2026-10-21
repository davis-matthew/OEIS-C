/*
 * A083520  Primes p such that p-1 is a product of two or more consecutive
 *          integers. Or (p-1) is a permutation of m items chosen from n, for
 *          some m and n. p-1 = k*(k+1)(k+2)...(k+r) for some k and r, r>0.
 * https://oeis.org/A083520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 85, u32 n <= 131, u64 n <= 131,
 *            u128 n <= 131, big any n
 * Helpers:   is_prime64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Function computes a(n) = the n-th prime p such that p-1 is a product
 * of at least two consecutive positive integers.  The sequence starts
 * with a(1)=3, a(2)=7, a(3)=13, … as listed in OEIS A083520. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/is_prime64.h"   /* bool is_prime64(uint64_t n); */

static bool has_representation(uint64_t m)
{
    /* m must be >=2, otherwise cannot be a product of >=2 consecutive ints. */
    if (m < 2) return false;

    /* length L = 2: solve k(k+1)=m */
    {
        uint64_t disc = 1ULL + 4ULL * m;
        uint64_t sqrt_disc = (uint64_t)(sqrtl((long double)disc) + 0.5L);
        if (sqrt_disc * sqrt_disc == disc && (sqrt_disc - 1) % 2 == 0)
            return true;
    }

    /* For lengths L >= 3 */
    unsigned __int128 fact = 2;    /* 2! for L = 2, will become L! when L increments */
    for (int L = 3; ; ++L)
    {
        fact *= (unsigned int)L;   /* now fact = L! */
        if (fact > m) break;       /* no product with k>=1 can reach m for this L */

        /* scan k from 1 upwards until product exceeds m */
        for (uint64_t k = 1; ; ++k)
        {
            unsigned __int128 prod = 1;
            for (int i = 0; i < L; ++i)
            {
                prod *= (unsigned __int128)(k + i);
                if (prod > m) break;
            }
            if (prod == m) return true;
            if (prod > m) break;   /* further k will only increase the product */
            /* If k became absurdly large (should not happen), break for safety */
            if (k > m) break;
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    static seq_t *values = NULL;
    static size_t cap = 0, len = 0;
    static uint64_t cand = 2;           /* candidate prime to test next */

    /* Ensure we have enough terms computed */
    while (len < n)
    {
        /* Find next prime satisfying the property */
        for (;; ++cand)
        {
            if (is_prime64(cand) && has_representation(cand - 1))
            {
                /* Grow storage if necessary */
                if (len == cap)
                {
                    size_t newcap = cap ? cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(values, newcap * sizeof(seq_t));
                    if (!tmp) return 0;   /* out of memory, return 0 as sentinel */
                    values = tmp;
                    cap = newcap;
                }
                values[len++] = (seq_t)cand;
                ++cand;                 /* start next search after this prime */
                break;
            }
        }
    }
    return values[n - 1];
}
