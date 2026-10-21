/*
 * A075490  a(n) = Sum_{j=1..n} phi(n)^j.
 * https://oeis.org/A075490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{j=1..n} phi(n)^j
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* oeis: A075490 */

/* Provide phi(n) and basic arithmetic on seq_t */
/* The build supplies the following headers:                      */
#include "../utils/phi.h"   /* provides seq_t phi(idx_t m) */

/* Exponentiation by squaring for seq_t */
static seq_t power_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & (idx_t)1) result *= base;
        base *= base;
        exp >>= (idx_t)1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return 0;          /* not used by OEIS, but safe */

    seq_t phi_n = phi(n);                  /* φ(n) */
    if (phi_n == (seq_t)1) {
        return (seq_t)n;                   /* sum = n when φ(n)=1 */
    }

    seq_t pow_n = power_seq_t(phi_n, n);   /* φ(n)^n */
    seq_t numerator = phi_n * (pow_n - (seq_t)1);
    seq_t denom     = phi_n - (seq_t)1;
    return numerator / denom;              /* geometric series sum */
}
