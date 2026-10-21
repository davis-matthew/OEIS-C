/*
 * A206927  Minimal numbers of binary length n+1 such that the number of
 *          contiguous palindromic bit patterns in the binary representation
 *          is minimal.
 * https://oeis.org/A206927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, changed
 * Formula:   a(n) = floor((37*2^(n+1)/63)) mod 2^(n+1)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalently, a(n) is the number whose binary representation is the
   first n+1 bits of the infinitely repeated pattern 100101. */
seq_t a(idx_t n)
{
    /* a(0) is unused; 0 is a safe return */
    if (n == 0)
        return (seq_t)0;

    /* The 6‑bit pattern 100101 */
    static const seq_t pattern[6] = { (seq_t)1, (seq_t)0, (seq_t)0,
                                      (seq_t)1, (seq_t)0, (seq_t)1 };

    seq_t result = (seq_t)0;
    idx_t i = 0;           /* bit counter (msb to lsb) */
    idx_t len = n + 1;     /* number of bits to generate */
    idx_t p = 0;           /* position in the pattern */

    while (i < len)
    {
        result = (result * (seq_t)2) | pattern[p];
        ++i;
        ++p;
        if (p == 6)
            p = 0;
    }

    return result;
}
