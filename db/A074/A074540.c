/*
 * A074540  a(n) = 2^n + 5^n + 9^n.
 * https://oeis.org/A074540
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n + 5^n + 9^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes base^exp using repeated multiplication.  The helper is
   static so it has internal linkage; the main function a() has
   external linkage as required. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t r = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        r *= base;
    return r;
}

seq_t a(idx_t n)
{
    return pow_seq((seq_t)2, n) + pow_seq((seq_t)5, n) + pow_seq((seq_t)9, n);
}
