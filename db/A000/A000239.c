/*
 * A000239  One-half of number of permutations of [n] with exactly one run of
 *          adjacent symbols differing by 1.
 * https://oeis.org/A000239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. = R(x)*h(x) + S(x), where h(x) = Sum_{k>=0}
 *            (k+1)!*(x*(1-x)/(1+x))^k, R = x*(1+x^2)/(1-x^2)^2 and S =
 *            x^2*(x^2-x-1)/((1+x)*(1-x)^2). h satisfies h(n) = (n+2)*h(n-1) -
 *            (n+1)*h(n-2) - (n-6)*h(n-3) + (n-2)*h(n-4) + p(n), with p(0)=1,
 *            p(1)=-1, p(2)=-3, p(3)=-1, p(n)=0 otherwise; h(n<0) = 0. Then...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t h1 = 0, h2 = 0, h3 = 0, h4 = 0; /* h(m-1), h(m-2), h(m-3), h(m-4) */
    seq_t g1 = 0, g2 = 0, g3 = 0, g4 = 0; /* a(m-1), a(m-2), a(m-3), a(m-4) */
    seq_t gm = 0;
    idx_t m;

    for (m = 0; m <= n; m++) {
        /* G(m) uses h(m-1), h(m-3) */
        gm = (seq_t)2 * g2 + h1 + h3;
        if (m == 5) gm += 1;
        gm -= g4;
        if (m == 2) gm -= 1;
        if (m == 3) gm -= 2;

        /* h(m) */
        seq_t hm = (seq_t)(m + 2) * h1;
        if (m >= 4) hm += (seq_t)(m - 2) * h4;
        if (m < 6) hm += (seq_t)(6 - m) * h3;
        if (m == 0) hm += 1;
        hm -= (seq_t)(m + 1) * h2;
        if (m > 6) hm -= (seq_t)(m - 6) * h3;
        if (m == 1) hm -= 1;
        if (m == 2) hm -= 3;
        if (m == 3) hm -= 1;

        h4 = h3; h3 = h2; h2 = h1; h1 = hm;
        g4 = g3; g3 = g2; g2 = g1; g1 = gm;
    }
    return gm;
}
