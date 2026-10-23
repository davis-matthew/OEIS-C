/*
 * A182194  a(1)=2, a(n)=a(n-1)^2 if the minimal natural number > 1 not yet in
 *          the sequence is greater than a(n-1), else a(n)=a(n-1)-1.
 * https://oeis.org/A182194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = b(m) + b(m-1) – n – 2 where b(1)=2, b(2)=3,
 *            b(k)=b(k-2)^2+1 and m = min{k | b(k) > n+1}. This follows the
 *            OEIS definition
 * Range:     u8 n <= 99, u16 n <= 9442, u32 n <= 9442, u64 n <= 9442,
 *            u128 n <= 9442, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* compute a(n) for n >= 1 */
seq_t a(idx_t n)
{
    /* static cache for the b sequence */
    static seq_t *b = NULL;      /* array of b values */
    static size_t b_len = 0;     /* number of stored elements */

    /* initialise the first two elements on first call */
    if (b_len == 0) {
        b_len = 2;
        b = (seq_t *)malloc(b_len * sizeof(seq_t));
        if (!b) return (seq_t)0;                 /* allocation failed, return 0 */
        b[0] = (seq_t)2;                     /* b(1) */
        b[1] = (seq_t)3;                     /* b(2) */
    }

    /* target is n + 1 as seq_t */
    seq_t target = (seq_t)n + 1;

    /* extend the b array until it contains a value > target */
    while (b[b_len - 1] <= target) {
        /* compute next value: b(k) = b(k-2)^2 + 1 */
        seq_t prev2 = b[b_len - 2];
        seq_t next = prev2 * prev2 + 1;

        /* reallocate to hold the new element */
        b = (seq_t *)realloc(b, (b_len + 1) * sizeof(seq_t));
        if (!b) return (seq_t)0;                 /* allocation failed, return 0 */
        b[b_len] = next;
        b_len++;
    }

    /* binary search for the smallest index m_idx such that b[m_idx] > target
       (array indices are 0‑based, b[m_idx] corresponds to b(m_idx+1)) */
    size_t left = 0, right = b_len - 1, m_idx = b_len; /* default to b_len if not found */
    while (left <= right) {
        size_t mid = left + (right - left) / 2;
        if (b[mid] > target) {
            m_idx = mid;
            if (mid == 0) break;
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }

    /* safety: m_idx must be at least 1 because b(1)=2 > 1 */
    if (m_idx == 0) m_idx = 1;

    /* retrieve b(m) and b(m-1) from the array */
    seq_t bm   = b[m_idx];           /* b(m)  */
    seq_t bm_1 = b[m_idx - 1];       /* b(m-1) */

    /* compute a(n) = b(m) + b(m-1) - n - 2 */
    seq_t n_plus_2 = (seq_t)n + 2;
    seq_t result = bm + bm_1 - n_plus_2;
    return result;
}
