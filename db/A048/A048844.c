/*
 * A048844  Least positive integer k for which 19^n divides k!.
 * https://oeis.org/A048844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive integer k such that 19^n divides k!
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case: 19^0 = 1 divides every factorial, but the
       sequence defines a(0) = 1 (the smallest positive integer). */
    if (n == 0)
        return (seq_t)1;

    /* Upper bound: k = 19*n always satisfies v_19(k!) >= n,
       because floor(k/19) = n and higher powers add non‑negative terms. */
    idx_t low = 1;
    idx_t high = 19 * n;  /* safe for n up to the limits of the tests */

    /* Binary search for the minimal k with exponent at least n. */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;

        /* Compute exponent of 19 in mid! */
        seq_t v = 0;
        idx_t t = mid;
        while (t) {
            t /= 19;
            v += (seq_t)t;
        }

        if (v >= (seq_t)n)
            high = mid;
        else
            low = mid + 1;
    }
    return (seq_t)low;
}
