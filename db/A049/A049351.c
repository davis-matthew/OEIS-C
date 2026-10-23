/*
 * A049351  Row sums of triangle A049327.
 * https://oeis.org/A049351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A049351 – Row sums of triangle A049327
 *
 *  The sequence satisfies the linear recurrence
 *
 *      a(n) = b(n) + Σ_{k=1}^{min(6,n-1)} b(k) * a(n-k)
 *
 *  where the convolution coefficients are
 *
 *        b(1)=1,  b(2)=15,  b(3)=120,
 *        b(4)=540, b(5)=1296, b(6)=1296,
 *        b(k)=0 for k>6 .
 *
 *  This is derived from the generating function
 *       G(x) = x*p(5,x) / (1-x*p(5,x))
 *  with   x*p(5,x) = x + 15x² + 120x³ + 540x⁴ + 1296x⁵ + 1296x⁶
 *
 *  The implementation keeps a memoised array so that
 *  the recurrence can be computed in O(n) time for any
 *  n≥1, regardless of the order of calls.
 *
 *  The code uses only integer arithmetic on the type
 *  seq_t, which may be an arbitrary‑precision unsigned
 *  integer class or a builtin unsigned type.  No
 *  external helper functions from the utilities list
 *  are required.
 */

#include <stdlib.h>

static const seq_t B[7] = {0, 1, 15, 120, 540, 1296, 1296};

static seq_t *memo   = NULL;   /* array a[0..top]   */
static size_t  top    = 0;      /* largest index computed */
static size_t  memsz  = 0;      /* allocated size of memo  */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset begins at 1 */
    size_t N = (size_t)n;

    /* grow the memoisation array if necessary */
    if (N >= memsz) {
        size_t newsz = N + 1;
        seq_t *tmp = (seq_t*)realloc(memo, newsz * sizeof(seq_t));
        if (!tmp) return 0;       /* allocation failure – return 0 */
        memo  = tmp;
        memsz = newsz;
    }

    /* compute values up to N */
    if (N > top) {
        size_t i;
        for (i = top + 1; i <= N; ++i) {
            seq_t res = 0;
            if (i <= 6)            /* add b(i) for i ≤ 6 */
                res += B[i];
            size_t kmax = (i > 6 ? 6 : i - 1);
            for (size_t k = 1; k <= kmax; ++k)
                res += B[k] * memo[i - k];
            memo[i] = res;
        }
        top = N;
    }

    return memo[N];
}
