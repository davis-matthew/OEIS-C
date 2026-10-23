/*
 * A043566  Number of runs in base-14 representation of n.
 * https://oeis.org/A043566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of runs in base‐14 representation of n
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                     /* "0" has one run */
    seq_t count = 1;                 /* at least one run */
    idx_t prev = n % 14;              /* least significant digit */
    n /= 14;
    while (n > 0) {
        idx_t digit = n % 14;
        if (digit != prev) {
            ++count;
            prev = digit;            /* start new run */
        }
        n /= 14;
    }
    return count;
}
