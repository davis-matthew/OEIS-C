/*
 * A043564  Number of runs in base-12 representation of n.
 * https://oeis.org/A043564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 + number of changes in adjacent base‑12 digits of n
 *            (with a(0) = 1). The function counts runs of equal digits in the
 *            base‑12 representation of n
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t runs = (seq_t)1;           /* first run */
    idx_t last = n % 12;             /* least‑significant digit */
    n /= 12;
    while (n > 0) {
        idx_t digit = n % 12;        /* next digit */
        if (digit != last) {        /* start of a new run */
            ++runs;
            last = digit;
        }
        n /= 12;
    }
    return runs;
}
