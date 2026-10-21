/*
 * A064868  The minimal number which has multiplicative persistence 4 in base
 *          n.
 * https://oeis.org/A064868
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 5*n - floor(n/24) for n > 23
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial terms for n = 5..23 are hardcoded. */
seq_t a(idx_t n)
{
    /* Values for n = 5..23 */
    static const seq_t init[19] = {
        2344, 172, 131, 174, 52, 77, 75, 83, 75,
        81, 89, 95, 101, 104, 110, 133, 143, 127, 133
    };

    if (n < 5) {
        /* The behavior for n < 5 is not specified; return 0. */
        return 0;
    }
    if (n <= 23) {
        return init[n - 5];
    }
    /* For n > 23 use the closed formula. */
    seq_t val = (seq_t)n * 5;
    return val - (seq_t)(n / 24);
}
