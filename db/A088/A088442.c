/*
 * A088442  A linear version of the Josephus problem.
 * https://oeis.org/A088442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + Sum_{odd j} bit_j(2*n+1) * 2^j i.e. take 2n+1, keep
 *            only its bits at odd positions, add 1
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 2*n + 1 in seq_t */
    seq_t v = ((seq_t)n << 1) + 1;

    seq_t odd_part = 0;
    /* start with bit 2^1 (the first odd position) */
    seq_t bit = ((seq_t)1 << 1);

    /* loop over odd bits only */
    while (bit <= v)
    {
        if (v & bit)
            odd_part += bit;

        /* prevent overflow when shifting left 2 bits */
        if (bit > v / 4)
            break;          /* next shift would exceed v or overflow */
        bit <<= 2;
    }
    return odd_part + (seq_t)1;
}
