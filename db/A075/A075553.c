/*
 * A075553  Numerators in the Maclaurin series for arctan(1+x).
 * https://oeis.org/A075553
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   a(n) = 0 if n % 4 == 0, a(n) = (-1)^((n-1)/4) if n % 4 == 1,
 *            a(n) = -(-1)^((n-2)/4) if n % 4 == 2, a(n) = (-1)^((n-3)/4) if n
 *            % 4 == 3. Equivalently the sequence is periodic with period 8:
 *            {0,1,-1,1,0,-1,1,-1}
 * Range:     i8 n <= 104, i16 n <= 104, i32 n <= 104, i64 n <= 104,
 *            i128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t tbl[8] = {0, 1, -1, 1, 0, -1, 1, -1};
    return tbl[n & 7u];
}
