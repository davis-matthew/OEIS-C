/*
 * A186438  Positive numbers whose squares end in two identical digits.
 * https://oeis.org/A186438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 100 * floor((n-1)/14) + pat[(n-1) % 14]; The sequence
 *            consists of all positive integers n such that n² ends in two
 *            identical digits. In ascending order, every block of 100 numbers
 *            contains the 14 values
 *            [10,12,20,30,38,40,50,60,62,70,80,88,90,100]
 * Range:     u8 n <= 35, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A186438. */
seq_t a(idx_t n)
{
    static const seq_t pat[14] = {10,12,20,30,38,40,50,60,62,70,80,88,90,100};
    idx_t k = n - 1;            /* adjust for 1-based indexing */
    idx_t q = k / 14;           /* number of complete 100‑blocks before this term */
    idx_t r = k % 14;           /* index within the current block */
    return pat[r] + (seq_t)100 * (seq_t)q;
}
