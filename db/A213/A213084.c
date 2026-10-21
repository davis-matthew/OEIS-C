/*
 * A213084  Numbers consisting of ones and eights.
 * https://oeis.org/A213084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, word
 * Formula:   a(n) = decimal value obtained by mapping the binary *
 *            representation of (n−1) with 0→1 and 1→8 (most significant * bit
 *            first). This yields the nth positive integer composed solely *
 *            of the digits 1 and 8, listed in increasing order
 * Range:     u8 n <= 10, u16 n <= 46, u32 n <= 1534, u64 n <= 9788,
 *            u128 n <= 9788, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Determine the length L of the number. */
    idx_t L = 0;
    idx_t val = n + 1;
    while (val > 1) {
        val >>= 1;
        ++L;
    }

    /* Compute 2^L – 1, the starting index for numbers of length L. */
    idx_t start_idx = 0;
    for (idx_t i = 0; i < L; ++i) {
        start_idx = start_idx * 2 + 1;   /* build 2^i+1 progressively */
    }

    /* Offset within the block of length L numbers. */
    idx_t offset = n - start_idx;

    /* Build the decimal value digit by digit. */
    seq_t result = 0;
    for (idx_t i = L; i > 0; --i) {
        int bit = (offset >> (i - 1)) & 1;
        result = result * 10 + (seq_t)(bit ? 8 : 1);
    }
    return result;
}
