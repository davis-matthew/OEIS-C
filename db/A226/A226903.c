/*
 * A226903  Shiraishi numbers: a parametrized family of solutions c to the
 *          Diophantine equation a^3 + b^3 + c^3 = d^3 with d = c+1.
 * https://oeis.org/A226903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (18*n^3 + 27*n^2 + 27*n + 1 - (3*n^2 + 3*n +
 *            1)*(-1)^n)/16
 * Range:     u8 n <= 5, u16 n <= 38, u32 n <= 1562, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*           a(2*k-1) = 9*k^3 - 6*k^2 + 3*k - 1 */
/*           a(2*k)   = 9*k^3 + 6*k^2 + 3*k */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined but harmless */

    /* use seq_t arithmetic for all intermediate values */
    seq_t rn = (seq_t)n;

    if ((rn & 1) != 0) {               /* odd n  => a(2*k-1) */
        seq_t k = (rn + 1) / 2;        /* k = (n+1)/2 */
        seq_t k2 = k * k;
        seq_t k3 = k2 * k;
        return 9 * k3 - 6 * k2 + 3 * k - 1;
    } else {                           /* even n => a(2*k)   */
        seq_t k = rn / 2;              /* k = n/2 */
        seq_t k2 = k * k;
        seq_t k3 = k2 * k;
        return 9 * k3 + 6 * k2 + 3 * k;
    }
}
