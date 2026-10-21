/*
 * A082902  a(n) = gcd(2^n, sigma(2,n)) = gcd(A000079(n), A001157(n)).
 * https://oeis.org/A082902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = gcd(2^n, σ₂(n)) = 2^min(n , v₂(σ₂(n))) where σ₂(n) =
 *            Σ_{d|n} d², and v₂(x) is the exponent of 2 in x
 * Range:     u8 n <= 9313, u16 n <= 9313, u32 n <= 9313, u64 n <= 9313,
 *            u128 n <= 9313, big any n
 * Helpers:   v2_of_sigma2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <stdint.h>   /* for uint64_t */
#include "../utils/v2_of_sigma2.h"

/* Compute 2^exp as a seq_t value.
   It is safe for seq_t types that are integer or arithmetic. */
static inline seq_t pow2_seq_t(unsigned exp)
{
    seq_t r = 1;
    for (unsigned i = 0; i < exp; ++i)
        r = r * (seq_t)2;
    return r;
}

/* OEIS sequence A082902 */
seq_t a(idx_t n)
{
    if (n == 0)   /* Not required for the sequence, but makes the function total */
        return (seq_t)1;

    unsigned exp_sigma2 = v2_of_sigma2(n);
    unsigned exp = (exp_sigma2 < (unsigned)n) ? exp_sigma2 : (unsigned)n;
    return pow2_seq_t(exp);
}
