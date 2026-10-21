/*
 * A078103  Number of maximal independent generating sets for S_n.
 * https://oeis.org/A078103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(n-2) + (n*(n-1)/2)*(n-1)^(n-3) for n > 6 (a(1)=1,
 *            a(2)=1, a(3)=6, a(4)=94, a(5)=285, a(6)=6342)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Small table for the first six terms.  Index 0 corresponds to a(1). */
static const seq_t small_vals[6] = {
    (seq_t)1,   /* a(1) */
    (seq_t)1,   /* a(2) */
    (seq_t)6,   /* a(3) */
    (seq_t)94,  /* a(4) */
    (seq_t)285, /* a(5) */
    (seq_t)6342 /* a(6) */
};

/* Exponentiation by squaring – pure integer math; works for any
   seq_t or uint64_t.  Catches the case exp == 0. */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp != (idx_t)0) {
        if (exp & (idx_t)1) {
            result *= base;
        }
        if (exp >>= (idx_t)1) {
            base *= base;
        }
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;  /* undefined for n = 0 – not used by the tests */
    }
    if (n <= (idx_t)6) {
        return small_vals[n - (idx_t)1];
    }

    /* Compute n^(n-2) */
    seq_t term1 = pow_seq_t((seq_t)n, n - (idx_t)2);

    /* Compute (n*(n-1)/2) */
    seq_t coeff = ((seq_t)n * ((seq_t)n - (seq_t)1)) / (seq_t)2;

    /* Compute (n-1)^(n-3) */
    seq_t term2_pow = pow_seq_t((seq_t)(n - (idx_t)1), n - (idx_t)3);

    seq_t term2 = coeff * term2_pow;

    return term1 + term2;
}
