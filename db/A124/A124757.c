/*
 * A124757  Zero-based weighted sum of compositions in standard order.
 * https://oeis.org/A124757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, look, tabf
 * Formula:   a(n) = sum of positions (starting at 1 for the LSB) of all 1
 *            bits in n * minus the position of the most significant 1. * That
 *            is, * a(n) = (Σ_{k ≥ 0, bit k of n = 1} (k+1)) - (⌊log₂ n⌋ + 1)
 *            for n > 0, * a(0) = 0. * This implements the definition from
 *            OEIS A124757
 * Range:     u8 n <= 9672, u16 n <= 9672, u32 n <= 9672, u64 n <= 9672,
 *            u128 n <= 9672, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* special case */
        return 0;

    uint64_t sum = 0;    /* use a wider type to avoid overflow on small seq_t */
    idx_t    pos = 1;    /* position of the current bit (1‑based) */
    idx_t    tmp = n;

    while (tmp != 0) {
        if (tmp & 1)          /* bit is set: add its position */
            sum += pos;
        pos++;                /* move to next bit position */
        tmp >>= 1;            /* shift right to process next bit */
    }

    /* pos is now one past the most significant 1; subtract its position */
    uint64_t highest_pos = pos - 1;
    sum -= highest_pos;

    return (seq_t)sum;  /* cast back to seq_t */
}
