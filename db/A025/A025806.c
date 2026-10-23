/*
 * A025806  Expansion of 1/((1-x^2)*(1-x^5)*(1-x^6)).
 * https://oeis.org/A025806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^2 + n*(13 + 5*(-1)^n) + 90 + 30*(-1)^n)/120)
 * Range:     u8 n <= 5, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t parity = n & 1U;          /* (-1)^n == 1 if parity==0 else -1 */
    seq_t nt = (seq_t)n;            /* promote index to seq_t type */

    seq_t top;
    if (parity == 0U) {             /* even n */
        top = nt * nt + 18 * nt + 120;
    } else {                        /* odd n */
        top = nt * nt + 8 * nt + 60;
    }

    return top / 120;               /* integer division truncates toward zero */
}
