/*
 * A011744  A binary m-sequence: expansion of reciprocal of x^31 + x^3 + 1
 *          (mod 2, shifted by 30 initial 0's).
 * https://oeis.org/A011744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-3) XOR a(n-31)  for n >= 31,
   with a(0..29) = 0 and a(30) = 1.  This is the binary 
   m‑sequence for the LFSR defined by x^31 + x^3 + 1. */
seq_t a(idx_t n)
{
    /* 30 leading zeros, a(30)=1 */
    if (n < 30)          return 0;
    if (n == 30)         return 1;

    /* Keep the last 31 values in a circular buffer. */
    seq_t buf[31];

    /* Initialise the first 31 values. */
    for (idx_t i = 0; i < 30; ++i)   buf[i] = 0;
    buf[30] = 1;

    seq_t result = 0;
    for (idx_t i = 31; i <= n; ++i)
    {
        result = buf[(i - 3) % 31] ^ buf[(i - 31) % 31];
        buf[i % 31] = result;
    }
    return result;
}
