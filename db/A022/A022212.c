/*
 * A022212  Gaussian binomial coefficients [ n,5 ] for q = 5.
 * https://oeis.org/A022212
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{i=1..5} (5^{\,n-i+1}-1)/(5^i-1) (q = 5)
 * Range:     u8 none, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use the pre‑existing big integer helper for exponentiation */
#include "../utils/seq_pow.h"

/* Gaussian binomial coefficient [n,5] for q = 5 */
seq_t a(idx_t n)
{
    /* The OEIS offset starts at n = 5.  For smaller n we return 0. */
    if (n < 5)
        return 0;

    /* 5^n as a seq_t.  All further numerator powers are obtained by
       dividing this value by 5 in each iteration. */
    seq_t pow5 = seq_pow((seq_t)5, n);
    seq_t result = 1;

    for (idx_t i = 1; i <= 5; ++i) {
        /* Numerator (5^{n-i+1} - 1) and denominator (5^i - 1) */
        seq_t num = pow5 - 1;
        seq_t den = seq_pow((seq_t)5, i) - 1;

        /* For native integer builds (≤ 64 bits) use a 128‑bit
           intermediate to avoid overflow.  For arbitrary‑precision
           big‑int builds the operator '/' works directly. */
        if (sizeof(seq_t) <= 8) {
            unsigned __int128 tmp = (unsigned __int128)result *
                                    (unsigned __int128)num;
            result = (seq_t)(tmp / (unsigned __int128)den);
        } else {
            result = result * num / den;
        }

        /* Prepare the next numerator value: divide by 5. */
        pow5 = pow5 / (seq_t)5;
    }

    return result;
}
