/*
 * A047537  Numbers that are congruent to {1, 4, 7} mod 8.
 * https://oeis.org/A047537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3k-2) = 8k-7, a(3k-1) = 8k-4, a(3k) = 8k-1 (equivalently
 *            numbers congruent to 1,4,7 mod 8)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1‑based; we compute based on n‑1 modulo 3 */
    idx_t g = (n - 1) / 3;          /* quotient 0‑based group */
    idx_t r = (n - 1) % 3;          /* remainder 0,1,2 */
    seq_t val;
    switch (r) {
        case 0: /* n = 3k-2  => a = 8k-7 = 8*(k-1)+1 = 8g + 1 */
            val = (seq_t)8 * (seq_t)g + (seq_t)1;
            break;
        case 1: /* n = 3k-1  => a = 8k-4 = 8*(k-1)+4 = 8g + 4 */
            val = (seq_t)8 * (seq_t)g + (seq_t)4;
            break;
        case 2: /* n = 3k    => a = 8k-1 = 8*(k)+7  = 8g + 7 */
            val = (seq_t)8 * (seq_t)g + (seq_t)7;
            break;
        default:
            val = (seq_t)0; /* unreachable */
            break;
    }
    return val;
}
