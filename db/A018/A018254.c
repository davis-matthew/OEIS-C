/*
 * A018254  Divisors of 28.
 * https://oeis.org/A018254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = A161713(n-1) (enumerate divisors of 28 in increasing
 *            order)
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static seq_t divs[6];
    static unsigned char ready = 0;

    if (!ready) {
        idx_t count = 0;
        idx_t limit = 28;                 /* the fixed integer whose divisors we need */
        for (idx_t i = 1; i <= limit; ++i) {
            if ((limit % i) == 0) {
                divs[count++] = (seq_t)i;
                if (count == 6) break;    /* 28 has exactly 6 divisors */
            }
        }
        ready = 1;
    }

    if (n >= 1 && n <= 6)
        return divs[n - 1];

    return (seq_t)0;                    /* undefined for n > 6 */
}
