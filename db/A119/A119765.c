/*
 * A119765  Irregular array where row n is the positive integers which divide
 *          the number of terms in all previous rows. a(1)=1.
 * https://oeis.org/A119765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A119765: a(n) is the n‑th element of the row‑major concatenation
   of irregular rows; row k contains all positive divisors of the total
   number of terms in rows 1 .. k‑1 (row 1 is fixed to 1). */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Comparator for qsort that orders idx_t values in ascending order. */
static int cmp_idx_t(const void *a, const void *b)
{
    idx_t va = *(const idx_t *)a;
    idx_t vb = *(const idx_t *)b;
    if (va < vb) return -1;
    if (va > vb) return  1;
    return 0;
}

seq_t a(idx_t n)
{
    /* static state for memoisation between calls */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;

    static idx_t tot = 0;        /* total number of terms processed so far   */

    /* buffer for divisor lists */
    static idx_t *div_buf = NULL;
    static size_t div_cap = 0;

    if (div_cap == 0) {
        div_cap = 64;
        div_buf = (idx_t *)malloc(div_cap * sizeof(idx_t));
    }

    /* extend cache until it contains at least n elements */
    while (cache_len < n) {
        /* Determine divisors of tot (total terms in previous rows) */
        size_t div_len = 0;
        if (tot == 0) {
            /* first row: fixed to [1] */
            div_len = 1;
            div_buf[0] = 1;
        } else {
            /* factorise tot */
            idx_t rem = tot;
            struct { idx_t p; int e; } fac[64];
            size_t primelen = 0;
            for (idx_t p = 2; p * p <= rem; ++p) {
                if (rem % p == 0) {
                    int e = 0;
                    while (rem % p == 0) {
                        rem /= p;
                        ++e;
                    }
                    fac[primelen].p = p;
                    fac[primelen].e = e;
                    ++primelen;
                }
            }
            if (rem > 1) {
                fac[primelen].p = rem;
                fac[primelen].e = 1;
                ++primelen;
            }

            /* generate all divisors */
            div_len = 1;
            div_buf[0] = 1;
            for (size_t i = 0; i < primelen; ++i) {
                idx_t p = fac[i].p;
                int e   = fac[i].e;
                size_t prev = div_len;
                idx_t pow = 1;
                for (int exp = 1; exp <= e; ++exp) {
                    pow *= p;
                    for (size_t j = 0; j < prev; ++j) {
                        if (div_len >= div_cap) {
                            div_cap *= 2;
                            div_buf = (idx_t *)realloc(div_buf, div_cap * sizeof(idx_t));
                        }
                        div_buf[div_len++] = div_buf[j] * pow;
                    }
                }
            }
            /* sort the divisors in ascending order */
            qsort(div_buf, div_len, sizeof(idx_t), cmp_idx_t);
        }

        /* append generated divisors to cache */
        for (size_t i = 0; i < div_len; ++i) {
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
                cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)div_buf[i];
        }

        /* update state for next row */
        tot += (idx_t)div_len;
    }

    return cache[n - 1];
}
