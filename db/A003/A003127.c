/*
 * A003127  Value of an urn with n balls of type -1 and n balls of type +1.
 * https://oeis.org/A003127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = B(n-1, n-1), where B(m,p) = C(m+p,m) * V(m,p) and V(m,p)
 *            is the value of an urn with m balls of -1 and p balls of +1:
 *            B(m,p) = max(0, B(m-1,p) + B(m,p-1) + C(m+p-1,m) -
 *            C(m+p-1,m-1)), B(0,p) = p, B(m,0) = 0. Computed by a row-by-row
 *            DP
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#define A3127_CAP 32768

static seq_t a3127_W[A3127_CAP];
static seq_t a3127_B[A3127_CAP];
static seq_t a3127_D[A3127_CAP];
static idx_t a3127_done = 0;

static void a3127_build(idx_t N) {
    idx_t m, p;
    for (p = 0; p <= N; p++) {
        a3127_W[p] = (seq_t)p;
        a3127_B[p] = 1;
    }
    a3127_D[0] = 0;
    for (m = 1; m <= N; m++) {
        a3127_W[0] = 0;
        a3127_B[0] = 1;
        for (p = 1; p <= N; p++) {
            seq_t c1 = a3127_B[p - 1]; /* C(m+p-1, m)   */
            seq_t c0 = a3127_B[p];     /* C(m+p-1, m-1) */
            seq_t x = a3127_W[p] + a3127_W[p - 1];
            if (c1 >= c0) {
                a3127_W[p] = x + (c1 - c0);
            } else {
                seq_t d = c0 - c1;
                if (x > d) a3127_W[p] = x - d;
                else a3127_W[p] = 0;
            }
            a3127_B[p] = c0 + c1;
        }
        a3127_D[m] = a3127_W[m];
    }
    a3127_done = N + 1;
}

seq_t a(idx_t n) {
    idx_t k, N;
    if (n == 0) return 0;
    k = n - 1;
    if (k >= A3127_CAP) return 0;
    if (k >= a3127_done) {
        N = 2 * a3127_done;
        if (N < k) N = k;
        if (N > A3127_CAP - 1) N = A3127_CAP - 1;
        a3127_build(N);
    }
    return a3127_D[k];
}
