/*
 * A225110  Numbers m such that S = Sum_{i = 1..q} 1/d(i) is an integer where
 *          d(i) are the divisors of m in increasing order, and q the smallest
 *          integer 1 < q <= tau(m) for m > 1; and a(1) = 1.
 * https://oeis.org/A225110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer m such that * S = Σ_{i=1..q} 1/d(i) is
 *            an integer for the smallest q>1 * where d(i) are the divisors of
 *            m in increasing order. * For m = 1, a(1) = 1 with q = 1
 * Range:     u8 n <= 3, u16 n <= 38, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, uint64_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/uint64_cmp.h"

/* ------------------------------------------------------------------ */
/*  Helper: GCD for generic seq_t                                   */
/* ------------------------------------------------------------------ */
static inline seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* ------------------------------------------------------------------ */
/*  Helper: dynamic array growth for seq_t array                     */
/* ------------------------------------------------------------------ */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        while (newcap < need) newcap <<= 1;
        *arr = (seq_t*)realloc(*arr, newcap * sizeof(seq_t));
        *cap = newcap;
    }
}

/* ------------------------------------------------------------------ */
/*  Comparator for qsort (uint64_t)                                  */

/* ------------------------------------------------------------------ */
/*  Global cache for a(n)                                            */
/* ------------------------------------------------------------------ */
static seq_t *seq_cache = NULL;
static size_t seq_cap = 0;   /* capacity of seq_cache */
static size_t seq_len = 0;   /* number of terms stored */

static idx_t last_checked = 1;   /* last m examined */

/* ------------------------------------------------------------------ */
/*  Ensure that seq_cache contains at least n terms                   */
/* ------------------------------------------------------------------ */
static void ensure_seq(size_t n)
{
    while (seq_len < n) {
        ++last_checked;
        if (last_checked == 1) continue;   /* skip m = 1, already cached */

        /* Factor last_checked into primes and exponents. */
        uint64_t pr[64];
        uint64_t ex[64];
        size_t cnt = 0;
        factor_u64((uint64_t)last_checked, pr, ex, &cnt);

        /* Generate all divisors (unsorted). */
        size_t div_cap = 16;
        uint64_t *divs = (uint64_t*)malloc(div_cap * sizeof(uint64_t));
        divs[0] = 1;
        size_t div_len = 1;
        for (size_t i = 0; i < cnt; ++i) {
            uint64_t p = pr[i];
            int e = (int)ex[i];
            size_t cur_len = div_len;
            uint64_t power = 1;
            for (int k = 1; k <= e; ++k) {
                power *= p;
                for (size_t j = 0; j < cur_len; ++j) {
                    uint64_t val = divs[j] * power;
                    if (div_len == div_cap) {
                        div_cap <<= 1;
                        divs = (uint64_t*)realloc(divs, div_cap * sizeof(uint64_t));
                    }
                    divs[div_len++] = val;
                }
            }
        }

        /* Sort divisors ascending. */
        qsort(divs, div_len, sizeof(uint64_t), uint64_cmp);

        /* Scan to find the smallest q>1 with integer sum of reciprocals. */
        seq_t num = 0, den = 1;
        bool good = false;
        for (size_t i = 0; i < div_len; ++i) {
            seq_t d = (seq_t)divs[i];
            seq_t nn = num * d + den;
            seq_t nd = den * d;
            seq_t g = gcd_seq(nn, nd);
            num = nn / g;
            den = nd / g;
            if (i + 1 >= 2 && den == 1) {  /* integer reached after at least two terms */
                good = true;
                break;
            }
        }
        free(divs);

        /* If condition satisfied, store the term. */
        if (good) {
            grow_seq(&seq_cache, &seq_cap, seq_len + 1);
            seq_cache[seq_len++] = (seq_t)last_checked;
        }
    }
}

/* ------------------------------------------------------------------ */
/*  Entry point: compute a(n)                                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (seq_len == 0) {               /* initialise cache for a(1) = 1 */
        grow_seq(&seq_cache, &seq_cap, 1);
        seq_cache[0] = (seq_t)1;
        seq_len = 1;
        last_checked = 1;
    }
    ensure_seq((size_t)n);
    return seq_cache[(size_t)(n - 1)];
}
