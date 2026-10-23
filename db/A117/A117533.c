/*
 * A117533  a(n) = Sum_{k=1..n} A117532(k).
 * https://oeis.org/A117533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=1..n} d(k) where d(k) is the smallest positive
 *            integer that has not been used before and for which a(k-1)+d(k)
 *            is coprime to k (i.e. gcd(a(k-1)+d(k),k)=1). a(0) is defined as
 *            0 for convenience
 * Range:     u8 n <= 22, u16 n <= 361, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/gcd.h"          /* provides seq_t gcd(seq_t, seq_t) */

seq_t a(idx_t n)
{
    /* static tables that grow as needed */
    static seq_t      *sum      = NULL;   /* sum[i] = a(i)  (sum[0] = a(0) = 0) */
    static size_t     sum_cap   = 0;       /* capacity of sum array   */
    static size_t     sum_size  = 0;       /* number of elements stored */

    static unsigned char *used  = NULL;   /* used[m] == 1 if m already chosen */
    static size_t          used_cap = 0;  /* capacity of used array   */

    /* first call: initialise */
    if (sum_size == 0) {
        sum_cap = 64;
        sum = (seq_t *)malloc(sum_cap * sizeof(seq_t));
        if (!sum) abort();
        sum[0] = (seq_t)0;          /* a(0) = 0 */
        sum_size = 1;

        used_cap = 16;
        used = (unsigned char *)calloc(used_cap, sizeof(unsigned char));
        if (!used) abort();
    }

    /* extend tables until a(n) is available */
    while (sum_size <= (size_t)n) {
        idx_t  k = (idx_t)sum_size;           /* current position (1‑based) */
        seq_t  prev = sum[sum_size - 1];      /* a(k-1)           */

        idx_t m = 1;                          /* candidate addend   */
        for (;;) {
            /* grow used array if necessary */
            if ((size_t)m >= used_cap) {
                size_t new_cap = used_cap ? used_cap * 2 : 16;
                if ((size_t)m >= new_cap)
                    new_cap = (size_t)m * 2;
                unsigned char *tmp = (unsigned char *)realloc(used, new_cap);
                if (!tmp) abort();
                memset(tmp + used_cap, 0, new_cap - used_cap);
                used  = tmp;
                used_cap = new_cap;
            }

            if (!used[(size_t)m]) {            /* m not used yet */
                seq_t candidate = prev + (seq_t)m;
                if (gcd(candidate, (seq_t)k) == (seq_t)1) {
                    used[(size_t)m] = 1;       /* mark as used            */
                    /* grow sum array if necessary */
                    if (sum_size == sum_cap) {
                        size_t new_cap = sum_cap * 2;
                        seq_t *tmp2 = (seq_t *)realloc(sum,
                                         new_cap * sizeof(seq_t));
                        if (!tmp2) abort();
                        sum = tmp2;
                        sum_cap = new_cap;
                    }
                    sum[sum_size] = candidate; /* a(k) = candidate       */
                    ++sum_size;
                    break;                     /* go on to next k         */
                }
            }
            ++m;
        }
    }

    return sum[(size_t)n];   /* a(n) is ready */
}
