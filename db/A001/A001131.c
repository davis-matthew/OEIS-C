/*
 * A001131  Number of red-black rooted trees with n-1 internal nodes.
 * https://oeis.org/A001131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. satisfies A(x) = x + x^2 + A((x+x^2)^2), i.e. a(n) = [n=1]
 *            + [n=2] + Sum_{n/4 <= m <= n/2} binomial(2m, n-2m)*a(m), a(0) =
 *            0, a(1) = 1, a(2) = 2. The rows (x+x^2)^(2m) are built
 *            incrementally, so only + and * are used
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#define A001131_MAXN 65536

static seq_t a001131_A[A001131_MAXN + 1];
static seq_t a001131_P[A001131_MAXN + 1];
static long long a001131_lim = -1;

static void a001131_build(long long N) {
    long long i, j, m;
    for (i = 0; i <= N; i++) {
        a001131_A[i] = 0;
        a001131_P[i] = 0;
    }
    if (N >= 1) a001131_A[1] = 1;
    if (N >= 2) a001131_A[2] = 1;
    a001131_P[0] = 1; /* (x+x^2)^0, stored by offset from degree 2m */
    for (m = 1; 2 * m <= N; m++) {
        long long L = 2 * m;
        seq_t am;
        if (N - 2 * m < L) L = N - 2 * m;
        /* multiply by (1 + x)^2 = 1 + 2x + x^2 (offsets shift by 2 in degree) */
        for (j = L; j >= 0; j--) {
            seq_t v = a001131_P[j];
            if (j >= 1) v += a001131_P[j - 1] * 2;
            if (j >= 2) v += a001131_P[j - 2];
            a001131_P[j] = v;
        }
        am = a001131_A[m];
        for (j = 0; j <= L; j++)
            a001131_A[2 * m + j] += am * a001131_P[j];
    }
    a001131_lim = N;
}

seq_t a(idx_t n) {
    long long nn;
    if (n == 0) return 0;
    if (n > A001131_MAXN) return 0;
    nn = (long long)n;
    if (nn > a001131_lim) {
        long long target = 2 * a001131_lim;
        if (target < 64) target = 64;
        if (target < nn) target = nn;
        if (target > A001131_MAXN) target = A001131_MAXN;
        a001131_build(target);
    }
    return a001131_A[nn];
}
