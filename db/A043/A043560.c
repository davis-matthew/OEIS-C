/*
 * A043560  Number of runs in base-8 representation of n.
 * https://oeis.org/A043560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of runs in the base‑8 representation of n
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case: the representation of 0 is "0", which has one run. */
    if (n == 0) {
        return 1;
    }

    /* Count runs while peeling off base‑8 digits from least significant. */
    seq_t runs = 1;                 /* At least one run exists.  */
    idx_t  prev = n & 7;             /* Current lowest digit.          */
    n >>= 3;                         /* Remove that digit.             */
    for (; n != 0; n >>= 3) {        /* Process remaining digits.     */
        idx_t digit = n & 7;
        if (digit != prev) {
            runs++;
        }
        prev = digit;
    }
    return runs;
}
