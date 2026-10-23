/*
 * A043556  Number of runs in base-4 representation of n.
 * https://oeis.org/A043556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of runs in the base‑4 representation of n (the
 *            number of maximal blocks of equal consecutive digits)
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)          /* the representation "0" counts as one run */
        return (seq_t)1;

    idx_t prev = (idx_t)4; /* sentinel – not a valid base‑4 digit (0…3) */
    seq_t runs = (seq_t)0;

    while (n) {
        idx_t digit = n & 3; /* n % 4 */
        if (digit != prev) {
            runs++;
            prev = digit;
        }
        n >>= 2;            /* n /= 4 */
    }
    return runs;
}
