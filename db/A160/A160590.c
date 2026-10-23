/*
 * A160590  Next-to-least significant digit of 2^n.
 * https://oeis.org/A160590
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor((2^n mod 100)/10)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4) {
        return 0;        /* undefined for n < 4, but harmless */
    }
    /* The second least significant decimal digit of 2^n is periodic
       with period 20, starting at n = 4. */
    static const seq_t table[20] = {
        1, 3, 6, 2, 5, 1, 2, 4, 9, 9,
        8, 6, 3, 7, 4, 8, 7, 5, 0, 0
    };
    return table[(n - 4) % 20];
}
