/*
 * A170479  Number of reduced words of length n in Coxeter group on 38
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^45 = I.
 * https://oeis.org/A170479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A170479: Number of reduced words of length n
   in a Coxeter group on 38 generators with (S_i)^2 = (S_i S_j)^45 = I.
   Formula:
       a(0) = 1
       a(n) = 38 * 37^(n-1)          for n >= 1
   (The sequence is geometric from n = 1 onward.) */

#include <stdint.h>

/* Helper: fast binary exponentiation of a seq_t value.
   Exponent is an idx_t (uint64_t).  Returns base^exp. */
static seq_t pow_seq(idx_t exp, seq_t base)
{
    seq_t result = (idx_t)1;
    seq_t power = base;

    while (exp > 0)
    {
        if (exp & 1)
            result *= power;
        power *= power;
        exp >>= 1;
    }
    return result;
}

/* Return the nth term of the sequence (zero‑based indexing). */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (idx_t)1;
    }

#if defined(__cplusplus)
    /* In C++ the seq_t type is typically a big‑integer class with a
       constructor from integer literals. */
    seq_t base37 = seq_t(37);
    seq_t coeff38 = seq_t(38);
#else
    /* In plain C the seq_t type is an unsigned integer type. */
    seq_t base37 = (seq_t)37;
    seq_t coeff38 = (seq_t)38;
#endif

    seq_t p = pow_seq(n - 1, base37);
    return coeff38 * p;
}
