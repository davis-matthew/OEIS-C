/*
 * A136313  a(1) = 1; for n>1, a(n) = a(n-1) + 8 mod 22.
 * https://oeis.org/A136313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*(n-1) mod 22 + 1, with a(1) = 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n == 0 the offset is not defined; return 0 */
    if (n == 0) return 0;

    /* Compute (n-1) mod 22 to avoid overflow */
    idx_t km = (n - 1) % 22;

    /* Compute 8 * km and reduce mod 22 */
    seq_t val = (seq_t)km * 8;
    val = val % 22;

    /* Add the initial value 1 */
    val = val + 1;

    return val;
}
