/*
 * A000314  Number of mixed Husimi trees with n nodes; or labeled polygonal
 *          cacti with bridges.
 * https://oeis.org/A000314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A035351(n)/n, where A035351(n) = (n-1)! [t^(n-1)]
 *            exp(n*f(t)) with f(t) = t + t^2/(2(1-t)) (Lagrange inversion of
 *            A = x exp(f(A))). u_k = k! [t^k] exp(n f) satisfies u_{k+1} =
 *            (2k+n) u_k - (k(k-1)+n k) u_{k-1} + n (k(k-1)/2) u_{k-2}, u_0 =
 *            1, u_{-1} = u_{-2} = 0, and a(n) = u_{n-1}/n, a(0)...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t nn = (seq_t)n;
    idx_t m = (n < 4) ? n - 1 : n - 2;

    seq_t p2 = 0, p1 = 0, c = 1; /* u_{k-2}, u_{k-1}, u_k */
    for (idx_t k = 0; k < m; k++) {
        seq_t pos = (seq_t)(2 * k) * c + nn * c
                  + nn * (seq_t)(k * (k - 1) / 2) * p2;
        seq_t neg = (seq_t)(k * (k - 1)) * p1 + nn * (seq_t)k * p1;
        seq_t nx = pos - neg;
        p2 = p1;
        p1 = c;
        c = nx;
    }

    if (n < 4)
        return c / nn;

    idx_t k = m;
    seq_t X = (seq_t)3 * c + (seq_t)(k * (k - 1) / 2) * p2;
    X -= (seq_t)(2 * n - 7) * p1;

    seq_t q1 = c / nn, r1 = c % nn;
    seq_t q2 = p1 / nn, r2 = p1 % nn;
    seq_t corr = (seq_t)4 * q1 + (seq_t)6 * q2
               + ((seq_t)4 * r1 + (seq_t)6 * r2) / nn;

    return X - corr;
}
