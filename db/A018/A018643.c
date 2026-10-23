/*
 * A018643  Divisors of 782.
 * https://oeis.org/A018643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) is the n‑th divisor of 782 in ascending order
 *            (1,2,17,23,34,46,391,782)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* Store the divisors of 782 in sorted order */
    static seq_t divisors[8];
    static idx_t initialized = 0;

    if (!initialized) {
        idx_t limit = 782;
        idx_t count = 0;
        for (idx_t i = 1; i <= limit; ++i) {
            if (limit % i == 0) {
                divisors[count++] = (seq_t)i;
            }
        }
        initialized = 1;
    }

    if (n > 8)      /* sequence is finite */
        return (seq_t)0;
    return divisors[n - 1];
}
