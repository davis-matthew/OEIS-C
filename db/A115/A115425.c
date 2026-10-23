/*
 * A115425  The first four terms of the sequence are doubled, then those
 *          numbers are tripled and then those numbers are quadrupled, etc.
 * https://oeis.org/A115425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * ((n % 4) + 1) * factorial((n / 4) + 1)
 * Range:     u8 n <= 16, u16 n <= 27, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Determine group index and multiplier within the group */
    idx_t group = n / 4;            /* number of completed groups */
    idx_t mult  = n % 4 + 1;        /* 1,2,3 or 4 within the current group */

    /* Compute (group + 1)! iteratively in seq_t; use (group + 1) to
       match the factorial for the current group (e.g., group=0 -> 1! = 1). */
    seq_t fact = 1;
    for (idx_t i = 2; i <= group + 1; ++i) {
        fact *= (seq_t)i;
    }

    /* Final result: 2 * mult * fact */
    seq_t result = 2;
    result *= (seq_t)mult;
    result *= fact;
    return result;
}
