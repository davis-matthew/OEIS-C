/*
 * A017783  Binomial coefficients C(67,n).
 * https://oeis.org/A017783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = binomial(67, n) (coefficients of (1 + x)^67)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Static cache holding the 68 coefficients of row 67 of Pascal's
       triangle.  The row is built once using only integer additions,
       which guarantees no overflow for any seq_t that can represent
       the 64‑bit values that arise. */
    static seq_t cache[68];
    static int initialized = 0;

    if (!initialized) {
        /* Initialise with row 0. */
        for (idx_t i = 0; i < 68; ++i) {
            cache[i] = 0;
        }
        cache[0] = 1;

        /* Build rows 1…67 in place; update from the end to avoid
           overwriting needed values.  After r == 67, cache[0…67] hold
           binomial(67,k). */
        for (idx_t r = 1; r <= 67; ++r) {
            for (idx_t k = r; k >= 1; --k) {
                cache[k] += cache[k - 1];
            }
        }
        initialized = 1;
    }

    return (n <= 67) ? cache[(idx_t)n] : (seq_t)0;
}
