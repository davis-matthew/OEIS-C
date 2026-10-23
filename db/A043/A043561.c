/*
 * A043561  Number of runs in base-9 representation of n.
 * https://oeis.org/A043561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of runs in the base‑9 representation of n
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case: 0 has a single run */
    if (n == 0)
        return (seq_t)1;
    seq_t runs = 1;           /* at least one run */
    unsigned char last = (unsigned char)(n % 9);
    n /= 9;
    while (n != 0) {
        unsigned char digit = (unsigned char)(n % 9);
        n /= 9;
        if (digit != last) {
            runs++;
            last = digit;
        }
    }
    return runs;
}
