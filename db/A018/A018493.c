/*
 * A018493  Divisors of 508.
 * https://oeis.org/A018493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the nth divisor of 508 in increasing order (a(1)=1,
 *            a(2)=2, a(3)=4, a(4)=127, a(5)=254, a(6)=508, a(n)=0 for n>6)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static int initialized = 0;
    static seq_t divisors[6];
    if (!initialized) {
        const idx_t limit = 508;
        idx_t i, count = 0;
        for (i = 1; i <= limit; ++i) {
            if (limit % i == 0) {
                divisors[count++] = (seq_t)i;
            }
        }
        initialized = 1;
    }
    if (n == 0 || n > 6) return 0;
    return divisors[n - 1];
}
