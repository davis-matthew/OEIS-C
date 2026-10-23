/*
 * A128265  a(n) = Sum_{m=1..n} lcm(s(n,m),S(n,m)), where s(n,m) is an
 *          unsigned Stirling number of the first kind and S(n,m) is a
 *          Stirling number of the second kind. (a(n) = sum of terms in n-th
 *          row of triangle A128264.)
 * https://oeis.org/A128265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{m=1..n} lcm(s(n,m), S(n,m)) where s(n,m) are
 *            unsigned Stirling numbers of the first kind and S(n,m) are
 *            Stirling numbers of the second kind
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Requires utilities for gcd, memory allocation and freeing of seq_t arrays */
#include "../utils/gcd.h"
#include "../utils/alloc_seq_array.h"   /* provides alloc_seq_array/alloc_seq */
#include "../utils/free_seq.h"          /* provides free_seq */

/* Compute a(n) for arbitrary n ≥ 1.  The function memoises all values
   computed so far so that arbitrary calls with decreasing n are still
   fast.  This implementation uses two temporary rows of
   Stirling numbers and reuses them for successive n.  */
seq_t a(idx_t n)
{
    /* static memoisation state */
    static seq_t *prev_s = NULL;          /* s(k-1, *) */
    static seq_t *prev_S = NULL;          /* S(k-1, *) */
    static idx_t  prev_k = 0;             /* last computed n */
    static seq_t *cache  = NULL;          /* a(1)..a(prev_k) */
    static size_t cache_size = 0;         /* cached length */

    if (n <= 0) {                         /* not part of the offset, safety */
        return (seq_t)0;
    }

    /* initialise row 0 once (s(0,0)=S(0,0)=1) */
    if (prev_k == 0 && prev_s == NULL) {
        prev_s = alloc_seq_array(1);
        prev_S = alloc_seq_array(1);
        prev_s[0] = prev_S[0] = (seq_t)1;
    }

    /* extend memoisation up to requested n */
    for (idx_t k = prev_k + 1; k <= n; ++k) {
        size_t len = (size_t)k + 1;

        /* allocate new temporary row */
        seq_t *s  = alloc_seq_array(len);
        seq_t *S  = alloc_seq_array(len);
        s[0] = S[0] = (seq_t)0;            /* not used, but harmless */

        /* generate Stirling numbers for current row using recurrence */
        for (idx_t m = 1; m <= k; ++m) {
            /* unsigned Stirling first kind */
            seq_t val_s = prev_s[m - 1];
            if (m <= prev_k) {
                val_s += ((seq_t)(k - 1)) * prev_s[m];
            }
            s[m] = val_s;

            /* Stirling second kind */
            seq_t val_S = prev_S[m - 1];
            if (m <= prev_k) {
                val_S += ((seq_t)m) * prev_S[m];
            }
            S[m] = val_S;
        }

        /* sum lcm(s(m), S(m)) over m = 1..k */
        seq_t row_sum = (seq_t)0;
        for (idx_t m = 1; m <= k; ++m) {
            seq_t g = gcd(s[m], S[m]);                    /* lcm = (a/g)*b */
            seq_t l = (s[m] / g) * S[m];
            row_sum += l;
        }

        /* memoise a(k) */
        if (cache_size < (size_t)k) {
            seq_t *new_cache = alloc_seq_array(k);
            for (size_t i = 0; i < cache_size; ++i) {
                new_cache[i] = cache[i];
            }
            free_seq(cache);
            cache = new_cache;
            cache_size = k;
        }
        cache[k - 1] = row_sum;

        /* recycle previous row buffers */
        free_seq(prev_s);
        free_seq(prev_S);

        prev_s = s;
        prev_S = S;
        prev_k = k;
    }

    return cache[n - 1];
}
