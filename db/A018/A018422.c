/*
 * A018422  Divisors of 376.
 * https://oeis.org/A018422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 376 in ascending order */
seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* offset is 1; below it is undefined */
    const seq_t base = (seq_t)376;
    idx_t count = 0;
    for (idx_t i = 1; i <= (idx_t)base; ++i) {
        if ((base % (seq_t)i) == 0) {
            ++count;
            if (count == n) return (seq_t)i;
        }
    }
    return 0;                       /* n beyond number of divisors */
}
