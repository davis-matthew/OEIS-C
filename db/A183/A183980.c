/*
 * A183980  1/4 the number of (n+1) X 4 binary arrays with all 2 X 2 subblock
 *          sums the same.
 * https://oeis.org/A183980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*2^(n/2-1) + 2^(n-1) + 6 (n even) a(n) = 2^(((n-5)/2)+3)
 *            + 2^(n-1) + 6 (n odd) (These are derived from the OEIS formulas
 *            a(n)= (3*2^(n/2)+2^n+12)/2 for even n and
 *            a(n)=2^((n-5)/2+3)+2^(n-1)+6 for odd n, avoiding division and
 *            subtraction)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* For safety when n is below the offset (1). */
    if (n == 0) return 0;

    /* The power 2^(n-1) is common to all n. */
    seq_t term2 = seq_pow((seq_t)2, n - 1);

    seq_t term1;
    if ((n & 1ULL) == 0) {                 /* even n */
        idx_t e1 = n / 2 - 1;              /* exponent for 2^(n/2-1) */
        term1 = seq_pow((seq_t)2, e1);     /* 2^(n/2-1) */
        term1 = (seq_t)3 * term1;          /* multiplied by 3 */
    } else {                               /* odd n */
        /* For odd n compute ((n-5)/2)+3 safely using signed arithmetic. */
        int64_t temp = (int64_t)n - 5;     /* may be negative when n<5 */
        int64_t e1_signed = temp / 2 + 3;  /* integer division truncates toward 0 */
        idx_t e1 = (idx_t)e1_signed;
        term1 = seq_pow((seq_t)2, e1);     /* 2^(((n-5)/2)+3) */
        /* No multiplication for odd n. */
    }

    /* a(n) = term1 + term2 + 6 */
    return term1 + term2 + (seq_t)6;
}
