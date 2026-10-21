/*
 * A081692  Define two sequences by A_n = mex{A_i,B_i : 0 <= i < n}, B_n =
 *          B_{n-1} + (A_n-A_{n-1})(A_n-A_{n-1}+1), where the mex of a set is
 *          the smallest nonnegative integer not in the set. Sequence gives
 *          A_n. B_n is in A081693.
 * https://oeis.org/A081692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = mex{a(i), b(i) : 0 <= i < n}, with a(0) = 0, b(0) = 0,
 *            b(n) = b(n-1) + (a(n)-a(n-1))*(a(n)-a(n-1)+1)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* fixed‑size cache (sufficient for n up to 10 000, as required) */
#define CACHE_MAX 20001
static seq_t a_cache[CACHE_MAX];
static seq_t b_cache[CACHE_MAX];
static idx_t cache_len = 0;   /* number of terms currently stored (first value is a_cache[0]) */

/* return a(n) */
seq_t a(idx_t n)
{
    /* compute terms up to n on demand */
    while (cache_len <= n) {
        idx_t i = cache_len;
        if (i == 0) {
            a_cache[0] = 0;
            b_cache[0] = 0;
        } else {
            /* find mex among all previous a and b values */
            seq_t mex = 0;
            while (true) {
                bool present = false;
                for (idx_t j = 0; j < i; ++j) {
                    if (a_cache[j] == mex || b_cache[j] == mex) {
                        present = true;
                        break;
                    }
                }
                if (!present) break;
                ++mex;
            }
            a_cache[i] = mex;
            seq_t d = a_cache[i] - a_cache[i-1];
            b_cache[i] = b_cache[i-1] + d * (d + 1);
        }
        ++cache_len;
    }
    return a_cache[n];
}
