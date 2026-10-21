/*
 * A070605  n^5 mod 21.
 * https://oeis.org/A070605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 mod 21
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* reduce n modulo 21 first – this avoids overflow for small seq_t types */
    seq_t m = (seq_t)(n % 21);   /* m is in 0..20 */

    /* compute m^5 */
    seq_t x = m;          /* m^1 */
    x = x * x;            /* m^2 */
    x = x * x;            /* m^4 */
    x = x * m;            /* m^5 */

    return x % 21;        /* result in 0..20 */
}
