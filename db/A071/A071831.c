/*
 * A071831  Frequency ratios for notes of C-major scale starting at c = 1
 *          (numerators).
 * https://oeis.org/A071831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   pow2, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A071831 – Frequency ratios for notes of C‑major scale
   (numerators).  
   Formula: for n ≥ 0,
        let k = n / 7  (integer division)
            r = n % 7
        each base octave ratio is
            1/1, 9/8, 5/4, 4/3, 3/2, 5/3, 15/8
        let base_numer[r] be the numerator of ratio r,
            exp2[r] be the exponent of 2 in the denominator of ratio r
        then
            a(n) = base_numer[r] * 2^( max(0, k - exp2[r]) )
   This satisfies the recurrence a(n+7)=2*a(n) for n≥21.
*/
#include <stdint.h>
#include "../utils/pow2.h"      /* seq_t pow2(idx_t exp);  */
#include "../utils/seq_mul.h"   /* seq_t seq_mul(seq_t a, seq_t b); */

static const seq_t base_numer[7] = {1, 9, 5, 4, 3, 5, 15};
/* exponent of 2 in the denominator of the base ratios */
static const unsigned exp2_in_denom[7] = {0, 3, 2, 0, 1, 0, 3};

seq_t a(idx_t n)
{
    /* compute base octave and octave offset */
    idx_t k = n / 7;
    unsigned r = (unsigned)(n % 7);

    /* how many powers of two to multiply */
    idx_t add = (k > exp2_in_denom[r]) ? (k - exp2_in_denom[r]) : 0;

    /* 2^add as seq_t */
    seq_t mult = pow2(add);

    /* final value */
    return seq_mul(base_numer[r], mult);
}
