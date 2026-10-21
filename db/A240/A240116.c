/*
 * A240116  Schoenheim lower bound L(n,5,2).
 * https://oeis.org/A240116
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil((n/5) * ceil((n-1)/4))
 * Range:     u8 n <= 31, u16 n <= 534, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 5) {
        return 0;  /* sequence starts at n = 5 */
    }
    /* ceil((n-1)/4) => (n-1 + 4 - 1) / 4 = (n + 2) / 4 */
    idx_t t = (n + 2) / 4;              /* t = ceil((n-1)/4) */
    seq_t prod = (seq_t)n * (seq_t)t;   /* n * t, cast to seq_t */
    /* ceil(prod/5) => (prod + 5 - 1) / 5 = (prod + 4) / 5 */
    seq_t res = (prod + 4) / 5;
    return res;
}
