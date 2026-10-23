/*
 * A188626  a(n) = n + (n-1)*2^(n-2).
 * https://oeis.org/A188626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + (n-1)*2^(n-2)
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 60, u128 n <= 123,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_mul.h"

/* Compute 2^exp in seq_t using binary exponentiation. */
static seq_t pow2_exp(idx_t exp)
{
    seq_t result = 1;          /* 2^0 = 1   */
    seq_t base   = 2;          /* 2^1 = 2   */

    while (exp > 0) {
        if (exp & 1) {
            result = seq_mul(result, base);
        }
        base = seq_mul(base, base);
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* The definition is valid for n ≥ 1.  For n = 0 we return 0
       (the behaviour for indices below the offset is irrelevant). */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;                 /* a(1) = 1 */

    seq_t pow = pow2_exp(n - 2);                 /* 2^(n-2)  */
    seq_t term = (seq_t)n + ((seq_t)(n - 1)) * pow;
    return term;
}
