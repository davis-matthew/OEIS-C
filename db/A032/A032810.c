/*
 * A032810  Numbers using only digits 2 and 3.
 * https://oeis.org/A032810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = decimal number obtained by mapping the binary
 *            representation of (n - (2^k - 2) - 1), where k =
 *            floor(log2(n+1)), onto digits 2 and 3 (0→2, 1→3). This produces
 *            the increasing list of numbers composed solely of the digits 2
 *            and 3
 * Range:     u8 n <= 10, u16 n <= 62, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* the sequence is defined for n ≥ 1 */

    /* compute k = floor(log2(n+1)) */
    idx_t k    = 0;
    idx_t tmp  = n + 1;
    while (tmp >>= 1) k++;

    /* starting index of numbers with length k (0‑based) */
    idx_t group_start = (((idx_t)1) << k) - 2;

    /* offset within the current length group (0‑based) */
    idx_t offset = n - group_start - 1;

    /* construct the number from most significant bit to least */
    seq_t val = 0;
    for (idx_t i = k; i-- > 0; )
    {
        idx_t bit = (offset >> i) & 1;
        val = val * 10 + (bit ? 3 : 2);
    }
    return val;
}
