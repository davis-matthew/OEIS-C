/*
 * A215484  a(n) = 21*a(n-2) + 7*a(n-3), with a(0)=a(1)=0, a(2)=9.
 * https://oeis.org/A215484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 21*a(n-2) + 7*a(n-3); a(0) = 0, a(1) = 0, a(2) = 9
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 15, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle small indices directly */
    if (n == 0 || n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)9;

    /* Maintain the last three terms:
       a_minus3 = a(i-3), a_minus2 = a(i-2), a_minus1 = a(i-1) */
    seq_t a_minus3 = (seq_t)0; /* a(0) */
    seq_t a_minus2 = (seq_t)0; /* a(1) */
    seq_t a_minus1 = (seq_t)9; /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = (seq_t)21 * a_minus2 + (seq_t)7 * a_minus3;
        a_minus3 = a_minus2;
        a_minus2 = a_minus1;
        a_minus1 = next;
    }

    return a_minus1;
}
