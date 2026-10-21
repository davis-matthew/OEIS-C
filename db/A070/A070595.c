/*
 * A070595  a(n) = n^5 mod 9.
 * https://oeis.org/A070595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 mod 9
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)(n % 9);   /* reduce base modulo 9 first */
    seq_t res  = 1;
    int exp    = 5;                /* exponent fixed to 5 */

    while (exp) {
        if (exp & 1)               /* if current bit of exponent is set */
            res = (res * base) % 9;
        base = (base * base) % 9;  /* square base modulo 9 */
        exp >>= 1;
    }
    return res;
}
