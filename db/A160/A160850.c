/*
 * A160850  Number of lines through at least 2 points of a 10 X n grid of
 *          points.
 * https://oeis.org/A160850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2)*(f(10,n,1)-f(10,n,2)) * where f(m,n,k) =
 *            Sum_{-n<kx<n, -m<ky<m, gcd(x,y)=1} (n-|kx|)*(m-|ky|) * with m =
 *            10
 * Range:     u8 n <= 3, u16 n <= 32, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: fast GCD for unsigned integers */
static inline idx_t gcd_idx(idx_t a, idx_t b) {
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Compute f(m=10,n,k) as described above */
static inline seq_t compute_f(idx_t n, int k) {
    if (n == 0) return 0;
    const idx_t M = 10;               /* m = 10 */
    idx_t xmax = (n - 1) / (idx_t)k;  /* maximum |x| such that |k*x|<n */
    idx_t ymax = (M - 1) / (idx_t)k;  /* maximum |y| such that |k*y|<m */
    seq_t sum = 0;

    for (idx_t ax = 0; ax <= xmax; ++ax) {
        seq_t term_x = (seq_t)k * (seq_t)ax;   /* |k*x|             */
        seq_t nt = (seq_t)n - term_x;           /* n - |k*x|          */
        unsigned int sign_x = (ax == 0 ? 1 : 2);   /* number of x signs */

        for (idx_t ay = 0; ay <= ymax; ++ay) {
            if (ax == 0 && ay == 0) continue;          /* skip (0,0) */
            if (gcd_idx(ax, ay) != 1U) continue;        /* gcd(|x|,|y|)=1 needed */

            seq_t term_y = (seq_t)k * (seq_t)ay;       /* |k*y|             */
            seq_t mt = (seq_t)M - term_y;              /* m - |k*y|          */
            unsigned int sign_y = (ay == 0 ? 1 : 2);   /* number of y signs */
            unsigned int count = sign_x * sign_y;     /* total sign combinations */
            sum += (seq_t)count * nt * mt;
        }
    }
    return sum;
}

seq_t a(idx_t n) {
    /* Special case n=0: no lines contain at least two points. */
    if (n == 0) return 0;

    seq_t f1 = compute_f(n, 1);
    seq_t f2 = compute_f(n, 2);
    return (f1 - f2) / 2;
}
