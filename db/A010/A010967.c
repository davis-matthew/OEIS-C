/*
 * A010967  a(n) = binomial coefficient C(n,14).
 * https://oeis.org/A010967
 *
 * Offset:    14    (a(14) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial coefficient C(n, 14) a(14) = 1 a(n) = a(n-1) * n
 *            / (n-14) for n > 14
 * Range:     u8 n <= 16, u16 n <= 20, u32 n <= 36, u64 n <= 150,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: compute (acc * mul) / div exactly.
   Uses 128‑bit intermediate if available and seq_t is 64‑bit,
   otherwise relies on seq_t arithmetic (sufficient for arbitrary
   precision types). */
static inline seq_t mul_div(seq_t acc, idx_t mul, idx_t div)
{
#if defined(__SIZEOF_INT128__)
    unsigned __int128 temp = (unsigned __int128)acc * mul;
    return (seq_t)(temp / div);
#else
    /* Fallback: may overflow if seq_t is 64‑bit and the
       intermediate product does not fit. This path is meant
       for arbitrary‑precision seq_t where multiplication is
       handled exactly. */
    return (acc * (seq_t)mul) / (seq_t)div;
#endif
}

/* Return the n‑th term of OEIS sequence A010967: C(n,14)
   Only defined for n >= 14; for smaller n it returns 0. */
seq_t a(idx_t n)
{
    if (n < 14)
        return (seq_t)0;            /* undefined below offset */

    seq_t result = (seq_t)1;
    /* Use the multiplicative formula:
       result = C(n,14) = Π_{i=1}^{14} (n-14+i)/i                */
    for (idx_t i = 1; i <= 14; ++i)
    {
        idx_t numerator = n - 14 + i;
        result = mul_div(result, numerator, i);
    }
    return result;
}
