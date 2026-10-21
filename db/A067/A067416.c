/*
 * A067416  Ninth column of triangle A067410.
 * https://oeis.org/A067416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(n) = 10 * 18^(n-1) for n >= 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset */
    if (n == 0)
        return (seq_t)1;  /* cast to seq_t */

    /* Compute 18^(n-1) via exponentiation by squaring */
    idx_t exp = n - 1;
    seq_t pow18 = (seq_t)1;
    seq_t base   = (seq_t)18;

    while (exp) {
        if (exp & 1u)
            pow18 = pow18 * base;
        base = base * base;
        exp >>= 1;
    }

    /* Multiply by 10 to get the term */
    return pow18 * (seq_t)10;
}
