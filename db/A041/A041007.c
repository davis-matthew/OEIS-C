/*
 * A041007  Denominators of continued fraction convergents to sqrt(6).
 * https://oeis.org/A041007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 *   a(n) = (3 + (-1)^n) * a(n-1) + a(n-2)
 *   a(0) = 1,  a(1) = 2
 *   (i.e., coefficient 4 for even n, 2 for odd n)
 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)2;

    seq_t a_nm2 = (seq_t)1; /* a(n-2) */
    seq_t a_nm1 = (seq_t)2; /* a(n-1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t coeff = (i & 1) ? (seq_t)2 : (seq_t)4; /* 2 if odd, 4 if even */
        seq_t cur = coeff * a_nm1 + a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }

    return a_nm1;
}
