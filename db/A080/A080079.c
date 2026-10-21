/*
 * A080079  Least number causing the longest carry sequence when adding
 *          numbers <= n to n in binary representation.
 * https://oeis.org/A080079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 2 * 2^⌊log₂(n)⌋ – n Equivalently, let p be the greatest
 *            power of two ≤ n; then a(n) = 2p – n (or a(n) = p – (n – p)
 *            which avoids intermediate overflow)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)                 /* offset starts at 1, guard for safety */
        return 0;

    /* Find the largest power of two not exceeding n. */
    idx_t p = 1;
    while (p <= (n >> 1))
        p <<= 1;

    /* a(n) = 2p – n = p – (n – p) with no overflow. */
    idx_t diff   = n - p;   /* 0 ≤ diff < p   */
    idx_t result = p - diff;

    return (seq_t)result;
}
