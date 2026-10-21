/*
 * A070474  a(n) = n^3 mod 12, n^5 mod 12.
 * https://oeis.org/A070474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 mod 12 (since n^5 ≡ n^3 (mod 12))
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 12 first – only the remainder matters */
    idx_t r = n % 12;

    /* Compute r^3 mod 12 using only integer arithmetic */
    seq_t x = (seq_t)r * (seq_t)r;   /* r^2   */
    x = x % 12;                      /* r^2 mod 12 */
    x = (x * (seq_t)r) % 12;         /* r^3 mod 12 */

    return x;
}
