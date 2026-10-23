/*
 * A115017  a(n) = largest triangular number dividing n.
 * https://oeis.org/A115017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max{ k(k+1)/2 | k(k+1)/2 divides n } Computed by
 *            searching triangular divisors in descending order
 * Range:     u8 n <= 274, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the largest k such that T_k = k(k+1)/2 <= n. */
    idx_t kmax = 1;
    for (;; ++kmax) {
        seq_t t = (seq_t)kmax * ((seq_t)kmax + 1) / 2;
        if (t > (seq_t)n) {
            /* t exceeded n; kmax has been incremented once too far. */
            --kmax;
            break;
        }
    }

    /* Search downward for the first triangular divisor. */
    for (idx_t k = kmax; k > 0; --k) {
        seq_t t = (seq_t)k * ((seq_t)k + 1) / 2;
        if ((seq_t)n % t == 0) {
            return t;
        }
    }
    /* n == 0 is not an allowed input; for completeness return 1. */
    return 1;
}
