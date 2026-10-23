/*
 * A011760  Elevator buttons in U.S.A.: Positive integers except 13.
 * https://oeis.org/A011760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = n + sign( floor( n/13 ) )
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  sign(x) = 0 if x==0, else 1 */

seq_t a(idx_t n)
{
    seq_t res = (seq_t)n;
    if (n / 13 > 0) {
        res += 1;
    }
    return res;
}
