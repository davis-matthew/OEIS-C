/*
 * A109053  a(n) = lcm(n,12).
 * https://oeis.org/A109053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = lcm(n,12) = n*12/gcd(n,12)
 * Range:     u8 n <= 22, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* gcd(n,12) can be found only from n mod 12 */
    static const unsigned char gcd_table[12] = {
        12, /* 0  mod 12 */
         1, /* 1  mod 12 */
         2, /* 2  mod 12 */
         3, /* 3  mod 12 */
         4, /* 4  mod 12 */
         1, /* 5  mod 12 */
         6, /* 6  mod 12 */
         1, /* 7  mod 12 */
         4, /* 8  mod 12 */
         3, /* 9  mod 12 */
         2, /* 10 mod 12 */
         1  /* 11 mod 12 */
    };

    unsigned char g = gcd_table[(unsigned)(n % 12)];
    return ((seq_t)(n / g)) * (seq_t)12;
}
