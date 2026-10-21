/*
 * A063946  Write n in binary and complement second bit (from the left), with
 *          a(0)=0 and a(1)=1.
 * https://oeis.org/A063946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = n XOR (1 << (floor(log2(n))-1)) for n>=2; a(0)=0, a(1)=1
 * Range:     u8 n <= 255, u16 n <= 9759, u32 n <= 9759, u64 n <= 9759,
 *            u128 n <= 9759, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)n;

    /* msb = floor(log2(n)) (0‑based) */
    unsigned int msb = 63 - __builtin_clzll(n);
    unsigned int mask_pos = msb - 1;

    seq_t mask = (seq_t)1 << mask_pos;
    return ((seq_t)n) ^ mask;
}
