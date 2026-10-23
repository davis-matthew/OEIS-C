/*
 * A040349  Continued fraction for sqrt(369).
 * https://oeis.org/A040349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A040349: Continued fraction for sqrt(369) */
seq_t a(idx_t n)
{
    /* a(0) = floor(sqrt(369)) = 19.
       For n >= 1 the continued fraction repeats every 12 terms:
       4,1,3,2,7,4,7,2,3,1,4,38
       and thereafter repeats these 12 terms. */
    if (n == 0)
        return (seq_t)19;
    static const seq_t period[12] = {
        4, 1, 3, 2, 7, 4, 7, 2, 3, 1, 4, 38
    };
    /* (n-1) % 12 fits into size_t, valid for all n >= 1 */
    return period[(n - 1) % 12];
}
