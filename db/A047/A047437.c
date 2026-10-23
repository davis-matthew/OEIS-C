/*
 * A047437  Numbers that are congruent to {0, 5, 6} mod 8.
 * https://oeis.org/A047437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3k) = 8k - 2 * a(3k-1) = 8k - 3 * a(3k-2) = 8k - 8 * (valid
 *            for all n ≥ 1)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute k = ceil(n / 3) = (n + 2) / 3 using integer division */
    seq_t k = ((seq_t)n + 2) / 3;

    /* Determine remainder of n modulo 3 */
    idx_t r = n % 3;   /* 0 → n = 3k, 1 → n = 3k-2, 2 → n = 3k-1 */

    seq_t result;
    switch (r) {
    case 0:        /* n = 3k   */
        result = 8 * k - 2;
        break;
    case 1:        /* n = 3k-2 */
        result = 8 * k - 8;
        break;
    case 2:        /* n = 3k-1 */
        result = 8 * k - 3;
        break;
    default:       /* never reached */
        result = 0;
    }
    return result;
}
