/*
 * A000635  Number of paraffins C_n H_{2n} X Y with n carbon atoms.
 * https://oeis.org/A000635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   A(x) = B(x)/(1-B(x)), i.e. a(n) = b(n) + Sum_{k=1..n-1}
 *            b(k)*a(n-k), a(0) = 0, where B(x) = x*(R(x)^2 + R(x^2))/2 is the
 *            g.f. of A000642 and R(x) = 1 + x*(R(x)^3 + 3*R(x)*R(x^2) +
 *            2*R(x^3))/6 is the g.f. of A000598 (r(0) = 1)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 45, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#define A635_CAP 20005

static seq_t r635[A635_CAP];  /* A000598: rooted trees, outdegree <= 3 */
static seq_t s635[A635_CAP];  /* R(x)^2 */
static seq_t b635[A635_CAP];  /* A000642 */
static seq_t v635[A635_CAP];  /* A000635 */
static long long done635 = -1;

static void a635_extend(long long n) {
    long long m, i, j;
    if (done635 < 0) {
        r635[0] = 1;
        s635[0] = 1;
        b635[0] = 0;
        v635[0] = 0;
        done635 = 0;
    }
    for (m = done635 + 1; m <= n; m++) {
        long long k = m - 1;
        seq_t t = 0, p = 0, q = 0, bb, av;
        /* s635[k] is already known (computed when r635[k] was set) */
        for (i = 0; i <= k; i++)
            t += r635[i] * s635[k - i];
        for (j = 0; 2 * j <= k; j++)
            p += r635[j] * r635[k - 2 * j];
        if (k % 3 == 0)
            q = r635[k / 3];
        r635[m] = (t + 3 * p + 2 * q) / 6;

        /* s635[m] = coefficient of x^m in R(x)^2 */
        {
            seq_t s = 0;
            for (i = 0; i <= m; i++)
                s += r635[i] * r635[m - i];
            s635[m] = s;
        }

        bb = s635[k];
        if (k % 2 == 0)
            bb += r635[k / 2];
        bb /= 2;
        b635[m] = bb;

        av = bb;
        for (i = 1; i < m; i++)
            av += b635[i] * v635[m - i];
        v635[m] = av;
        done635 = m;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n >= (idx_t)(A635_CAP - 1)) return 0;
    if ((long long)n > done635)
        a635_extend((long long)n);
    return v635[n];
}
