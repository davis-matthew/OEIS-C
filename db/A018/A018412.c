/*
 * A018412  Divisors of 360.
 * https://oeis.org/A018412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th positive divisor of 360 in ascending order
 * Range:     u8 n <= 2, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* 360 = 2^3 * 3^2 * 5^1  → 24 divisors in total.  */
    if (n == 0) return 0;          /* below offset – unspecified but safe to return 0 */
    seq_t target = n;               /* target index (1‑based) */
    seq_t current = 1;              /* first divisor is 1 */
    seq_t count   = 1;              /* we have seen the first divisor */

    /* Special case: if n == 1, return 1 immediately. */
    if (target == 1) return 1;

    /* Iterate over all potential divisors from 2 to 360. */
    for (seq_t i = 2; i <= (seq_t)360; ++i) {
        if (((seq_t)360) % i == 0) {
            ++count;
            if (count == target) return i;
        }
    }

    /* If n is larger than the total number of divisors (24),
       return 0 (undefined value beyond the known sequence). */
    return 0;
}
