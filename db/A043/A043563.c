/*
 * A043563  Number of runs in base-11 representation of n.
 * https://oeis.org/A043563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   number of runs in the base‑11 representation of n
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case for n = 0: "0" has one run */
    if (n == 0)
        return 1;

    /* Determine first digit */
    idx_t prev = n % 11;
    n /= 11;

    seq_t runs = 1;  /* first run already counted */

    /* Iterate over remaining digits, counting transitions */
    while (n != 0) {
        idx_t cur = n % 11;
        if (cur != prev)
            ++runs;
        prev = cur;
        n /= 11;
    }

    return runs;
}
