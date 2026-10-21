/*
 * A214493  Numbers of the form ((6k+5)^2+9)/2 or 2(3k+4)^2-9.
 * https://oeis.org/A214493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n*(3*n+10)+27*(-1)^n+41)/4
 * Range:     u8 n <= 1, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t   = (seq_t)n;            /* cast index to sequence type */
    seq_t base = 6 * t * (3 * t + 10);
    seq_t num  = base + 41;
    if ((n & 1ULL) == 0) {           /* (-1)^n = +1 if n even, -1 if odd */
        num += 27;
    } else {
        num -= 27;
    }
    return num / 4;                  /* exact division by 4 */
}
