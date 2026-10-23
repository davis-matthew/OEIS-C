/*
 * A136713  At step n the sequence lists the number of occurrences of digit (n
 *          mod k), with k>0, in all the numbers from 1 to n. Case k=9.
 * https://oeis.org/A136713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   a(n) = number of occurrences of the decimal digit ((n+1) mod 9)
 *            in all integers from 1 to (n+1). (a(0) = 1.)
 * Range:     u8 n <= 755, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* m = n+1 according to OEIS offset convention */
    idx_t m = n + 1;
    idx_t d = m % 9;           /* target digit 0‑8 */

    seq_t count = 0;
    for (idx_t i = 1; i <= m; ++i) {
        idx_t x = i;
        while (x > 0) {
            if ((x % 10) == d) {
                ++count;
            }
            x /= 10;
        }
    }
    return count;
}
