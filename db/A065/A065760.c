/*
 * A065760  Concatenation of increasing number of alternating digits in base
 *          2, starting with 1.
 * https://oeis.org/A065760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(1)=1 a(n) = a(n‑1) * 2^n if n is even a(n) = (a(n‑1)+1) * 2^n
 *            – 1 if n is odd (equivalently: shift left by n and add 2^n‑1
 *            when n is odd)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* values for n < 1 are undefined – callers should use n >= 1 */
    seq_t val = 1;                    /* a(1) = 1 */
    if (n <= 1) return val;

    for (idx_t i = 2; i <= n; ++i) {
        /* shift left by i bits (i.e. multiply by 2^i) */
        val <<= i;

        /* for odd i, add the block of n ones */
        if (i & 1U) {
            seq_t ones = ((seq_t)1 << i) - 1;   /* 2^i – 1  */
            val += ones;
        }
    }
    return val;
}
