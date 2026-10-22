/*
 * A000624  Number of monosubstituted alkanes C(n)H(2n+1)-X of the form shown
 *          in the Comments lines that are not stereoisomers.
 * https://oeis.org/A000624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^(n-1)] (B(x^2)-1)*(B(x)-1), where B(x) = 1 +
 *            x*B(x^2)*B(x), i.e. B(0) = 1, B(m) = Sum_{j>=0, 2j<=m-1}
 *            B(j)*B(m-1-2j) (B counts alkyl radicals with no stereocenters;
 *            the tertiary carbon needs two identical branches among its three
 *            alkyl groups)
 * Range:     u8 n <= 14, u16 n <= 25, u32 n <= 46, u64 n <= 89, u128 n <= 174,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#define A000624_MAXN 32768

static seq_t a000624_B[A000624_MAXN];
static long long a000624_have = 0;

seq_t a(idx_t n)
{
    long long need, m, j, N;
    seq_t r;
    if (n < 4)
        return 0;
    N = (long long)n;
    need = N - 2;
    if (a000624_have == 0) {
        a000624_B[0] = 1;
        a000624_have = 1;
    }
    while (a000624_have <= need) {
        seq_t s = 0;
        m = a000624_have;
        for (j = 0; 2 * j <= m - 1; j++)
            s += a000624_B[j] * a000624_B[m - 1 - 2 * j];
        a000624_B[m] = s;
        a000624_have++;
    }
    r = 0;
    for (j = 1; 2 * j <= N - 2; j++)
        r += a000624_B[j] * a000624_B[N - 1 - 2 * j];
    return r;
}
