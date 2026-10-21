/*
 * A055640  Number of nonzero digits in decimal expansion of n.
 * https://oeis.org/A055640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   a(n) = the number of non‑zero decimal digits of n. Special case:
 *            a(0) = 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    while (n > 0) {
        if ((n % 10) != 0)
            count++;
        n /= 10;
    }
    return count;
}
