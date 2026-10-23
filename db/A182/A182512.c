/*
 * A182512  a(n) = (16^n - 1)/5.
 * https://oeis.org/A182512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (16^n - 1)/5
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base case: n = 0 gives 0 */
    if (n == 0) return 0;

    seq_t base = 16;      /* Current power of 16 to square */
    seq_t result = 1;     /* Accumulated product, initially 16^0 */

    idx_t exp = n;
    while (exp) {
        if (exp & 1) {
            result *= base;   /* multiply by current base if bit set */
        }
        base *= base;         /* square base for next bit */
        exp >>= 1;            /* move to next bit */
    }

    /* result now holds 16^n. Compute (16^n - 1)/5 */
    return (result - 1) / (seq_t)5;
}
