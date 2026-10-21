/*
 * A208508  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A208509; see the Formula section.
 * https://oeis.org/A208508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   A208508 is the triangle of coefficients of the polynomials
 *            u(n,x) defined by u(1,x)=1, v(1,x)=1 u(n,x)=u(n-1,x)+x·v(n-1,x)
 *            v(n,x)=u(n-1,x)+v(n-1,x)+1 The sequence is obtained by writing
 *            the rows of u(n,x) left‑to‑right. For an index n (offset 1) let
 *            m be the smallest integer with S(m)=∑_{i=1}^{m}...
 * Range:     u8 n <= 31, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* locate the row m and the position p in that row */
    idx_t pos = n - 1;
    idx_t m   = 1;
    idx_t cumulative = 0;
    idx_t p;

    while (1) {
        idx_t len = (m / 2) + 1;       /* length of row m */
        if (pos < cumulative + len) {
            p = pos - cumulative;
            break;
        }
        cumulative += len;
        ++m;
    }

    /* compute coefficients of u(m,x) */
    /* maximum degree needed for m≈200 is 100; 256 is safe */
    const idx_t MAX = 256;
    seq_t u[MAX], v[MAX], new_u[MAX], new_v[MAX];
    idx_t len_u = 1, len_v = 1;

    /* initial values */
    for (idx_t i = 0; i < MAX; ++i)
        u[i] = v[i] = 0;
    u[0] = 1;
    v[0] = 1;

    for (idx_t step = 2; step <= m; ++step) {
        /* compute new lengths */
        idx_t new_len_u = len_u > len_v + 1 ? len_u : len_v + 1;
        idx_t new_len_v = len_u > len_v ? len_u : len_v;

        /* zero new arrays up to their lengths */
        for (idx_t i = 0; i < new_len_u; ++i) new_u[i] = 0;
        for (idx_t i = 0; i < new_len_v; ++i) new_v[i] = 0;

        /* new_u = u + x*v */
        for (idx_t i = 0; i < len_u; ++i) new_u[i] += u[i];
        for (idx_t i = 0; i < len_v; ++i) new_u[i + 1] += v[i];

        /* new_v = u + v + 1 (constant term increased by 1) */
        for (idx_t i = 0; i < len_u; ++i) new_v[i] += u[i];
        for (idx_t i = 0; i < len_v; ++i) new_v[i] += v[i];
        new_v[0] += 1;

        /* copy back to current arrays */
        for (idx_t i = 0; i < new_len_u; ++i) u[i] = new_u[i];
        for (idx_t i = 0; i < new_len_v; ++i) v[i] = new_v[i];

        len_u = new_len_u;
        len_v = new_len_v;
    }

    return u[p];
}
