/*
 * A092322  Sum of largest parts of all partitions of n into odd parts.
 * https://oeis.org/A092322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 50, u32 n <= 176, u64 n <= 653,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A092322: Sum of largest parts of all partitions of n into
   odd parts.
   Algorithm: Dynamic programming over odd part sizes.
   For each odd part p, all partitions of m that contain at least one p
   are formed by appending p to a partition of m-p whose largest part
   is ≤ p.  The number of such new partitions is f[m-p] and each of them
   contributes p to the total sum.  Summing over all odd p gives the
   desired value.
   Complexity: O(n²) additions and multiplications, fast for n up to 10⁴.
*/

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1, guard against 0 */

    idx_t N = n;
    seq_t *f   = NULL;
    seq_t *sum = NULL;

#ifdef __cplusplus
    /* In C++ we need value‑initialised objects: use new[] */
    try {
        f   = new seq_t[N + 1]();   /* all elements zeroed */
        sum = new seq_t[N + 1]();
    } catch (...) {
        return (seq_t)0;   /* allocation failed */
    }
#else
    /* In C use malloc and explicit zeroing */
    f   = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    sum = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!f || !sum) {
        free(f); free(sum);
        return (seq_t)0;
    }
    for (idx_t i = 0; i <= N; ++i) {
        f[i]   = (seq_t)0;
        sum[i] = (seq_t)0;
    }
#endif

    f[0] = (seq_t)1;  /* one way to partition zero */

    for (idx_t p = 1; p <= N; p += 2) {   /* iterate over odd parts */
        for (idx_t m = p; m <= N; ++m) {
            seq_t add = f[m - p];
            f[m] += add;
            sum[m] += (seq_t)p * add;
        }
    }

    seq_t result = sum[N];

#ifdef __cplusplus
    delete [] f;
    delete [] sum;
#else
    free(f);
    free(sum);
#endif
    return result;
}
