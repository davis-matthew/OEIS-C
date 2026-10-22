/*
 * A000640  Number of paraffins C_n H_{2n-1} XYZ with n carbon atoms.
 * https://oeis.org/A000640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f. A(x) = x*T(x)/(1-P(x))^3, where T(x) = A000598(x) satisfies
 *            T(x) = 1 + x*(T(x)^3 + 3*T(x)*T(x^2) + 2*T(x^3))/6, and P(x) =
 *            A000642(x) = x*(T(x)^2 + T(x^2))/2
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#define A640_MAXN 20005

static seq_t A640_T[A640_MAXN], A640_T2[A640_MAXN], A640_P[A640_MAXN];
static seq_t A640_S[A640_MAXN], A640_S2[A640_MAXN], A640_S3[A640_MAXN];
static long long A640_done = 0; /* number of indices computed */

static void a640_extend(long long m)
{
    long long i;
    if (m == 0) {
        A640_T[0] = 1; A640_T2[0] = 1; A640_P[0] = 0;
        A640_S[0] = 1; A640_S2[0] = 1; A640_S3[0] = 1;
        return;
    }
    {
        long long j = m - 1;
        seq_t s = 0, t = 0;
        for (i = 0; i <= j; i++) s += A640_T[i] * A640_T2[j - i];
        for (i = 0; 2 * i <= j; i++) t += A640_T[j - 2 * i] * A640_T[i];
        s += 3 * t;
        if (j % 3 == 0) s += 2 * A640_T[j / 3];
        A640_T[m] = s / 6;
    }
    {
        seq_t s = 0;
        for (i = 0; i <= m; i++) s += A640_T[i] * A640_T[m - i];
        A640_T2[m] = s;
    }
    {
        long long j = m - 1;
        seq_t s = A640_T2[j];
        if (j % 2 == 0) s += A640_T[j / 2];
        A640_P[m] = s / 2;
    }
    {
        seq_t s = 0;
        for (i = 1; i <= m; i++) s += A640_P[i] * A640_S[m - i];
        A640_S[m] = s;
    }
    {
        seq_t s = 0;
        for (i = 0; i <= m; i++) s += A640_S[i] * A640_S[m - i];
        A640_S2[m] = s;
    }
    {
        seq_t s = 0;
        for (i = 0; i <= m; i++) s += A640_S2[i] * A640_S[m - i];
        A640_S3[m] = s;
    }
}

seq_t a(idx_t n)
{
    long long N, k;
    seq_t r = 0;
    if (n == 0) return 0;
    N = (long long)n;
    if (N > A640_MAXN - 1) N = A640_MAXN - 1;
    while (A640_done < N) {
        a640_extend(A640_done);
        A640_done++;
    }
    for (k = 0; k <= N - 1; k++) r += A640_T[k] * A640_S3[N - 1 - k];
    return r;
}
