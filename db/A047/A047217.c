/*
 * A047217  Numbers that are congruent to {0, 1, 2} mod 5.
 * https://oeis.org/A047217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*floor((n-1)/3) + ((n-1)%3) (n >= 1)
 * Range:     u8 n <= 154, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1-based index, so n-1 fits in idx_t for all valid n. */
    idx_t t = n - 1;                /* t = n-1  */
    idx_t k = t / 3;                /* group index */
    idx_t r = t % 3;                /* position within group (0,1,2) */

    /* Convert to seq_t and compute. */
    seq_t sk = (seq_t)k;
    seq_t sr = (seq_t)r;
    seq_t five = (seq_t)5;

    return five * sk + sr;
}
