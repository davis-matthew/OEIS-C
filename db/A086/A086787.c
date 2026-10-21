/*
 * A086787  a(n) = Sum_{i=1..n} ( Sum_{j=1..n} i^j ).
 * https://oeis.org/A086787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n} ( Sum_{j=1..n} i^j )
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Helper: fast exponentiation with seq_t base and idx_t exponent.
   Assumes seq_t supports +, *, -, / and literals. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t b = base;
    idx_t e = exp;
    while (e) {
        if (e & 1) result = result * b;
        b = b * b;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* a(1) = 1, because Sum_{j=1..1} 1^j = 1 */
    seq_t total = (seq_t)n;               /* term for i = 1, which is n */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t base = (seq_t)i;
        seq_t pow_i = pow_seq(base, n);           /* i^n   */
        seq_t term = base * (pow_i - (seq_t)1) / (base - (seq_t)1);
        total += term;
    }
    return total;
}
