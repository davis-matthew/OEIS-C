/*
 * A227044  a(n) = Sum_{k>=1} k^(2*n)/(2^k).
 * https://oeis.org/A227044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k>=1} k^(2*n)/(2^k) (equivalently a(n) =
 *            Sum_{k=0..2*n} (-2)^k * k! * Stirling2(2*n, k) )
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *a_cache = NULL;          /* a_cache[i] holds a(i) */
    static size_t a_cache_len = 0;          /* number of elements allocated */

    /* dynamic programming over rows of modified Stirling numbers T(m,k) = k! * S(m,k) */
    static seq_t *t_prev = NULL;            /* previous row (m-1) */
    static size_t t_prev_len = 0;           /* length of t_prev = m_prev + 1 */
    static idx_t current_m = 0;             /* current maximum m processed */

    /* Ensure a_cache can hold n */
    if ((size_t)n >= a_cache_len) {
        size_t new_len = (size_t)(n + 1);
        a_cache = (seq_t *)realloc(a_cache, new_len * sizeof(seq_t));
        a_cache_len = new_len;
    }

    /* Initialize for m = 0 if necessary */
    if (t_prev == NULL) {
        t_prev_len = 1;
        t_prev = (seq_t *)calloc(t_prev_len, sizeof(seq_t));
        t_prev[0] = 1;                       /* T(0,0) = 1 */
        current_m = 0;
        if ((size_t)0 >= a_cache_len) {
            a_cache = (seq_t *)realloc(a_cache, sizeof(seq_t));
            a_cache_len = 1;
        }
        a_cache[0] = 1;                       /* a(0) = 1 */
    }

    /* Target m = 2*n */
    idx_t target_m = 2 * n;
    if (current_m < target_m) {
        /* Grow DP from current_m+1 up to target_m */
        for (idx_t m = current_m + 1; m <= target_m; ++m) {
            size_t len = (size_t)(m + 1);
            seq_t *t_cur = (seq_t *)calloc(len, sizeof(seq_t));
            /* Compute t_cur[1..m] using recurrence k * (T_prev[k-1] + T_prev[k]) */
            for (size_t k = 1; k <= (size_t)m; ++k) {
                seq_t a0 = (k - 1 < t_prev_len) ? t_prev[k - 1] : 0;
                seq_t a1 = (k < t_prev_len) ? t_prev[k] : 0;
                t_cur[k] = ((seq_t)k) * (a0 + a1);
            }

            /* If m is even, compute a(m/2) from this row */
            if ((m & 1ULL) == 0) {
                idx_t idx = m >> 1;
                if ((size_t)idx >= a_cache_len) {
                    size_t new_len = (size_t)(idx + 1);
                    a_cache = (seq_t *)realloc(a_cache, new_len * sizeof(seq_t));
                    a_cache_len = new_len;
                }
                /* Sum_{k} (-2)^k * T_cur[k] = sum_even - sum_odd,
                   where sum_even/odd are weighted by 2^k */
                seq_t sum_even = 0;
                seq_t sum_odd  = 0;
                seq_t pow2 = 1;                 /* 2^0 */
                for (size_t k = 1; k <= (size_t)m; ++k) {
                    pow2 = pow2 << 1;          /* 2^k */
                    if ((k & 1ULL) == 0)
                        sum_even += pow2 * t_cur[k];
                    else
                        sum_odd  += pow2 * t_cur[k];
                }
                a_cache[idx] = sum_even - sum_odd;
            }

            /* Advance DP */
            free(t_prev);
            t_prev = t_cur;
            t_prev_len = len;
            current_m = m;
        }
    }

    return a_cache[(size_t)n];
}
