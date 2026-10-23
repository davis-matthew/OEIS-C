/*
 * A012020  Expansion of e.g.f.: sin(sin(arctan(x))) (odd powers only).
 * https://oeis.org/A012020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1, a(1)=-4, a(2)=76; a(n) = -4*(3*n^2-3*n+1)*a(n-1)
 *            -12*(n-1)^2*(2*n-3)*(2*n-1)*a(n-2)
 *            -4*(n-2)*(n-1)*(2*n-5)*(2*n-3)^2*(2*n-1)*a(n-3) for n≥3
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 5, i64 n <= 9, i128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-4;
    if (n == 2) return (seq_t)76;

    seq_t a0 = (seq_t)1;          /* a(n-3) */
    seq_t a1 = (seq_t)-4;         /* a(n-2) */
    seq_t a2 = (seq_t)76;         /* a(n-1) */
    seq_t curr;

    for (idx_t i = 3; i <= n; ++i) {
        __int128 n128 = (__int128)i;

        __int128 coeff1 = - (__int128)4 * (3*n128*n128 - 3*n128 + 1);
        __int128 coeff2 = - (__int128)12 * (n128-1)*(n128-1) *
                          (2*n128-3)*(2*n128-1);
        __int128 coeff3 = - (__int128)4 * (n128-2)*(n128-1) *
                          (2*n128-5) * (2*n128-3) * (2*n128-3) *
                          (2*n128-1);

        __int128 prod1 = coeff1 * (__int128)a2; /* a(n-1) */
        __int128 prod2 = coeff2 * (__int128)a1; /* a(n-2) */
        __int128 prod3 = coeff3 * (__int128)a0; /* a(n-3) */

        __int128 total = prod1 + prod2 + prod3;
        curr = (seq_t)total;

        a0 = a1;
        a1 = a2;
        a2 = curr;
    }
    return a2;  /* a(n) */
}
