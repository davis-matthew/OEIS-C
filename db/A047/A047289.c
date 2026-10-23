/*
 * A047289  Numbers that are congruent to {0, 4, 6} mod 7.
 * https://oeis.org/A047289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3k) = 7k - 1 * a(3k-1) = 7k - 3 * a(3k-2) = 7k - 7 * (where k
 *            = ceil(n/3))
 * Range:     u8 n <= 109, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Since n >= 1, no special handling of offset zero is required. */
    idx_t k = (n + 2) / 3;   /* k = ceil(n/3) with integer arithmetic */

    switch ((unsigned)n % 3) {   /* n%3 fits in unsigned short */
        case 0: {   /* n = 3k   */
            return 7 * (seq_t)k - 1;
        }
        case 1: {   /* n = 3k-2 */
            return 7 * (seq_t)k - 7;
        }
        case 2: {   /* n = 3k-1 */
            return 7 * (seq_t)k - 3;
        }
        /* No default case needed because every n >= 1 falls into one of the above. */
    }
    /* Unreachable, but return 0 to satisfy compiler in case of a compile-time anomaly. */
    return 0;
}
