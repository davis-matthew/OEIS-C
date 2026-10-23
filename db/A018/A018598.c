/*
 * A018598  Divisors of 700.
 * https://oeis.org/A018598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   enumerate divisors of 700 in increasing order
 * Range:     u8 n <= 16, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 700
 * a(1) = 1, a(2) = 2, a(3) = 4, a(4) = 5, a(5) = 7, a(6) = 10,
 * a(7) = 14, a(8) = 20, a(9) = 25, a(10) = 28, a(11) = 35,
 * a(12) = 50, a(13) = 70, a(14) = 100, a(15) = 140, a(16) = 175,
 * a(17) = 350, a(18) = 700
 */
seq_t a(idx_t n)
{
    const idx_t LIMIT = 700;
    if (n < 1) return 0;
    idx_t count = 0;
    for (idx_t d = 1; d <= LIMIT; ++d) {
        if (LIMIT % d == 0) {
            ++count;
            if (count == n) {
                return (seq_t)d;
            }
        }
    }
    return 0; /* undefined for n > 18 */
}
