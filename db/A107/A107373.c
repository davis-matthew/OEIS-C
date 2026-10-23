/*
 * A107373  a(n) = (n/2)*binomial(n-1, floor((n-1)/2)) - 2^(n-2).
 * https://oeis.org/A107373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 32, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS Sequence A107373
   Formula: a(n) = (n/2) * binomial(n-1, floor((n-1)/2)) - 2^(n-2)
   (the division by 2 is exact: either n or the binomial coefficient is even) */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n <= 1)              /* offset is 1, a(1)=0 */
        return 0;

    idx_t k   = (n - 1) / 2;  /* floor((n-1)/2) */
    seq_t comb = binomial(n - 1, k);

    /* compute n*comb/2 without overflowing:
       – if n is even, use (n/2)*comb
       – if n is odd, comb is even, so use (comb/2)*n            */
    seq_t term;
    if (n & 1U) {            /* n is odd */
        term = (comb / 2) * (seq_t)n;
    } else {                 /* n is even */
        term = comb * ((seq_t)n / 2);
    }

    seq_t pow = pow2(n - 2);  /* 2^(n-2) – exact because n>=2 here */
    return term - pow;
}
