/*
 * A078621  Values of n such that Sum[ -(-1)^(k) n/k (n-1)/(k+1),{k,1,n}]
 *          (n!!) is an integer.
 * https://oeis.org/A078621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (4 - n%2) * 2^floor((n-1)/2) + 1 (for n > 4) a(1)=1,
 *            a(2)=5, a(3)=9, a(4)=11
 * Range:     u8 n <= 13, u16 n <= 29, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the OEIS sequence A078621 */
seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)9;
    if (n == 4) return (seq_t)11;

    /* Compute 2^floor((n-1)/2) */
    idx_t e = (n - 1) / 2;
    seq_t pow = (seq_t)1;
    for (idx_t i = 0; i < e; ++i)
        pow <<= 1;   /* safe because loop runs only until overflow hits,
                      but overflow only matters for values that don't fit
                      into the built-in integer size, which are irrelevant
                      for the 64‑bit test cases. */

    /* multiplier is 4 for even n, 3 for odd n */
    seq_t mult = (seq_t)(4 - (n & 1));

    return mult * pow + (seq_t)1;
}
