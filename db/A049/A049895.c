/*
 * A049895  a(n) = a(1) + a(2) + ... + a(n-1) - a(m) for n >= 4, where m = 2*n
 *          - 2 - 2^(p+1) and p is the unique integer such that 2^p < n - 1 <=
 *          2^(p+1), starting with a(1) = a(2) = 1 and a(3) = 3.
 * https://oeis.org/A049895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A049895:
   a(1)=1, a(2)=1, a(3)=3
   For n≥4:
      let p be the unique integer with 2^p < n‑1 ≤ 2^(p+1)
      compute m = 2*n – 2 – 2^(p+1)
      a(n) = a(1)+⋯+a(n‑1) – a(m)
 */
#include <stdlib.h>
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    /* static cache of computed terms and their prefix sums */
    static seq_t *a_arr = NULL;
    static seq_t *s_arr = NULL;
    static idx_t cached = 0;          /* number of terms currently stored */

    if (n == 0)                       /* undefined; return 0 */
        return (seq_t)0;

    if (n > cached) {
        /* allocate or grow the arrays */
#if defined(__cplusplus)
        a_arr = reinterpret_cast<seq_t*>(realloc(a_arr, n * sizeof(seq_t)));
        s_arr = reinterpret_cast<seq_t*>(realloc(s_arr, n * sizeof(seq_t)));
#else
        a_arr = (seq_t*)realloc(a_arr, n * sizeof(seq_t));
        s_arr = (seq_t*)realloc(s_arr, n * sizeof(seq_t));
#endif
        /* compute all missing terms */
        for (idx_t i = cached + 1; i <= n; ++i) {
            seq_t val;
            if (i == 1) {
                val = (seq_t)1;
            } else if (i == 2) {
                val = (seq_t)1;
            } else if (i == 3) {
                val = (seq_t)3;
            } else {
                idx_t p      = floor_log2_uint64(i - 2);
                idx_t pow2   = ((idx_t)1) << (p + 1);
                idx_t m      = 2 * i - 2 - pow2;      /* m < i by definition */
                seq_t a_m    = a_arr[m - 1];
                seq_t sumprev = s_arr[i - 2];          /* sum of a(1)..a(i-1) */
                val = sumprev - a_m;
            }
            a_arr[i - 1] = val;
            if (i == 1)
                s_arr[0] = val;
            else
                s_arr[i - 1] = s_arr[i - 2] + val;
        }
        cached = n;
    }

    return a_arr[n - 1];
}
