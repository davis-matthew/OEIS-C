/*
 * A097384  Total number of comparisons to find each of the values 1 through n
 *          using a binary search with 3-way comparisons (less than, equal and
 *          greater than), always choosing the mid-most value to compare to.
 * https://oeis.org/A097384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n + a(floor((n-1)/2)) + a(ceil((n-1)/2)), a(1)=0 The
 *            recurrence can be rewritten as a(n) = n + a[(n-1)/2] + a[n/2]
 *            for all n >= 0, with a(0)=a(1)=0. This implementation computes
 *            the value iteratively in O(n) time and O(n) memory per call. It
 *            works for any integer or arbitrary‑precision seq...
 * Range:     u8 n <= 56, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* safety for internal use */
    /* allocate temporary array of size n+1; type seq_t is integer or
       arbitrary‑precision class, which is trivially copyable.  */
    seq_t *tmp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!tmp) return (seq_t)0;            /* allocation failure → 0 */
    tmp[0] = (seq_t)0;
    tmp[1] = (seq_t)0;
    for (idx_t i = 2; i <= n; ++i) {
        idx_t left  = (i - 1) / 2;        /* floor((i-1)/2) */
        idx_t right = i / 2;              /* ceil((i-1)/2) for all i */
        seq_t val = (seq_t)i + tmp[left] + tmp[right];
        tmp[i] = val;
    }
    seq_t result = tmp[n];
    free(tmp);
    return result;
}
