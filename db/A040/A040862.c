/*
 * A040862  Continued fraction for sqrt(892).
 * https://oeis.org/A040862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A040862: continued fraction terms for √892.
   a(0)=29; for n≥1, a(n) follows the repeating pattern
   [1,6,2,14,2,6,1,58].
   The sequence is computed on-demand with O(1) time. */
seq_t a(idx_t n)
{
    /* First term is the integer part of √892. */
    if (n == 0) {
        return (seq_t)29;
    }

    /* Periodic part after the first term. */
    /* The pattern length is 8. */
    static const seq_t period[8] = {
        (seq_t)1, (seq_t)6, (seq_t)2, (seq_t)14,
        (seq_t)2, (seq_t)6, (seq_t)1, (seq_t)58
    };

    /* Compute the index within the period. */
    idx_t idx = (n - 1) % 8;
    return period[idx];
}
