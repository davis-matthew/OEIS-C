/*
 * A043558  Number of runs in base-6 representation of n.
 * https://oeis.org/A043558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of runs in the base-6 representation of n.
   A run is a maximal consecutive block of equal digits.
   For n = 0 the representation is "0", which has one run. */
seq_t a(idx_t n)
{
    /* Special case: n = 0 → representation "0" → one run */
    if (n == 0) {
        return 1;
    }

    seq_t runs = 1;         /* At least one run (the most significant digit) */
    idx_t prev_digit = n % 6;
    n /= 6;                 /* move to the next, less significant digit */

    while (n) {
        idx_t d = n % 6;
        if (d != prev_digit) {
            ++runs;
            prev_digit = d;
        }
        n /= 6;
    }
    return runs;
}
