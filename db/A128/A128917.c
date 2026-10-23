/*
 * A128917  Pentagonal numbers (A000326) which are also centered pentagonal
 *          numbers (A005891).
 * https://oeis.org/A128917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (5*(s(n)^2 - s(n)) + 2) / 2 where s(n) = (y(n)+1)/2, y(n)
 *            is the coefficient of √15 in (5+√15)·(4+√15)^(n-1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* not used; safety */

    /* handle the first term separately */
    if (n == 1) {
        seq_t A = 1;   /* a_0 of (4+√15)^k */
        seq_t B = 0;   /* b_0 of (4+√15)^k */
        seq_t y = A + 5 * B;    /* y(0) = 1 */
        seq_t s = (y + 1) / 2;  /* s(0) = 1 */
        seq_t t = s * s - s;
        return (5 * t + 2) / 2; /* a(1) = 1 */
    }

    /* for n ≥ 2 we need a_{n-1} and b_{n-1} of (4+√15)^k */
    seq_t Ap = 1;        /* A_prev = a_0 */
    seq_t Ac = 4;        /* A_cur  = a_1 */
    seq_t Bp = 0;        /* B_prev = b_0 */
    seq_t Bc = 1;        /* B_cur  = b_1 */

    for (idx_t i = 2; i < n; ++i) {
        seq_t An = 8 * Ac - Ap;  /* a_{i} = 8*a_{i-1} - a_{i-2} */
        seq_t Bn = 8 * Bc - Bp;  /* b_{i} = 8*b_{i-1} - b_{i-2} */
        Ap = Ac;  Ac = An;
        Bp = Bc;  Bc = Bn;
    }

    /* Ac, Bc now correspond to a_{n-1}, b_{n-1} */
    seq_t y = Ac + 5 * Bc;           /* coefficient of √15 */
    seq_t s = (y + 1) / 2;           /* (y+1)/2 */
    seq_t t = s * s - s;             /* s^2 - s */
    return (5 * t + 2) / 2;          /* final term a(n) */
}
