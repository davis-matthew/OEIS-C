/*
 * A174183  a(n) is the period k such that binomial(m, n) (mod 10) =
 *          binomial(m + k, n) (mod 10).
 * https://oeis.org/A174183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(0) = 1; a(n) = 10 * n! for n ≥ 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Compute n! as a product of 1*2*...*n */
    seq_t fact = 1;          /* factorial for n (fact = n!) */
    if (n >= 2) {            /* start from 2, since 1! = 1 */
        for (idx_t i = 2; i <= n; ++i) {
            fact *= (seq_t)i;
        }
    }
    /* If n == 0 handle separately: a(0) = 1 */
    if (n == 0) {
        return 1;
    }
    /* For n ≥ 1: a(n) = 10 * n! */
    return fact * (seq_t)10;
}
