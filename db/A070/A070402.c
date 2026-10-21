/*
 * A070402  a(n) = 2^n mod 5.
 * https://oeis.org/A070402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 2^n mod 5
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Sequence repeats every 4: 1, 2, 4, 3 */
    static const seq_t tbl[4] = {
        (seq_t)1,
        (seq_t)2,
        (seq_t)4,
        (seq_t)3
    };
    /* idx_t is unsigned, so bitwise & works for n % 4 */
    idx_t r = n & 3ULL;
    return tbl[r];
}
