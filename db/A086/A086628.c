/*
 * A086628  Antidiagonal sums of square table A086626.
 * https://oeis.org/A086628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=2; for n>=2 a(n) = 2 + Σ_{i=0}^{n-2} a(i) *
 *            B(n-2-i), where B(k) = Σ_{j=0}^k a(j) * a(k-j)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* n is non‑negative */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    size_t N = (size_t)n;          /* safe for n <= 10000 */
    size_t Bsize = 2 * N + 1;      /* to hold indices up to 2N */
    seq_t *a_vals = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *B      = (seq_t *)calloc(Bsize, sizeof(seq_t));
    if (!a_vals || !B) {
        /* In production this should never happen for n <= 10000. */
        return (seq_t)0;
    }

    /* Base values */
    a_vals[0] = (seq_t)1;
    B[0] = a_vals[0] * a_vals[0];   /* B(0) = a0 * a0 */

    /* Add a[1] = 2 and update B */
    a_vals[1] = (seq_t)2;
    {
        size_t cur = 1;
        for (size_t j = 0; j <= cur; ++j) {
            if (j == cur)
                B[cur + j] += a_vals[cur] * a_vals[cur];
            else
                B[cur + j] += (a_vals[cur] * a_vals[j]) * (seq_t)2;
        }
    }

    /* Compute the rest of the sequence */
    for (size_t cur = 2; cur <= N; ++cur) {
        /* Compute a[cur] */
        seq_t sum = 0;
        size_t m = cur - 2;
        for (size_t i = 0; i <= m; ++i)
            sum += a_vals[i] * B[m - i];
        a_vals[cur] = (seq_t)2 + sum;

        /* Update B with the new value a[cur] */
        for (size_t j = 0; j <= cur; ++j) {
            if (j == cur)
                B[cur + j] += a_vals[cur] * a_vals[cur];
            else
                B[cur + j] += (a_vals[cur] * a_vals[j]) * (seq_t)2;
        }
    }

    seq_t result = a_vals[N];
    free(a_vals);
    free(B);
    return result;
}
