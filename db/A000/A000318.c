/*
 * A000318  Generalized tangent numbers d(4,n).
 * https://oeis.org/A000318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 2^(4n-2) * A000182(n), where the tangent numbers
 *            A000182(n) come from the Brent-Zimmermann recurrence: T(1)=1,
 *            T(k)=(k-1)*T(k-1), then for k=2..N, j=k..N: T(j) = (j-k)*T(j-1)
 *            + (j-k+2)*T(j)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#define A318_MAXN 20005

static seq_t a318_work[A318_MAXN + 1];
static seq_t a318_tan[A318_MAXN + 1];
static idx_t a318_have = 0;

static void a318_build(idx_t N)
{
    idx_t j, k;
    a318_work[1] = 1;
    for (k = 2; k <= N; k++)
        a318_work[k] = a318_work[k - 1] * (seq_t)(k - 1);
    for (k = 2; k <= N; k++)
        for (j = k; j <= N; j++)
            a318_work[j] = a318_work[j - 1] * (seq_t)(j - k)
                         + a318_work[j] * (seq_t)(j - k + 2);
    for (k = 1; k <= N; k++)
        a318_tan[k] = a318_work[k];
    a318_have = N;
}

seq_t a(idx_t n)
{
    seq_t r;
    idx_t i;
    if (n < 1 || n > A318_MAXN)
        return 0;
    if (n > a318_have) {
        idx_t N = n;
        if (a318_have > 0 && N < 2 * a318_have)
            N = 2 * a318_have;
        if (N < 64)
            N = 64;
        if (N > A318_MAXN)
            N = A318_MAXN;
        a318_build(N);
    }
    r = a318_tan[n];
    r <<= 2;
    for (i = 1; i < n; i++)
        r <<= 4;
    return r;
}
