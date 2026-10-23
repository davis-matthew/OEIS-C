/*
 * A100212  Expansion of 4*x^4*(2 + x)/(1 - 2*x + 2*x^2 - 4*x^4 + 8*x^5 -
 *          8*x^6).
 * https://oeis.org/A100212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(8k+4)=2^(4k+3) a(8k+5)=5*2^(4k+2) a(8k+6)=3*2^(4k+3)
 *            a(8k+7)=2^(4k+3) a(n)=0 otherwise
 * Range:     u8 n <= 12, u16 n <= 28, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n) {
    /* Handle the zero first block */
    if (n < 4)
        return 0;

    /* Calculate n mod 8 */
    idx_t r = n & 7;
    if (r < 4)
        return 0;          /* n ≡ 0,1,2,3 (mod 8) -> 0 */

    /* For r = 4,5,6,7 compute k and the exponent */
    idx_t k  = (n - 4) / 8;            /* k = floor((n-4)/8)          */
    idx_t exp = 4 * k + 3;             /* exponent for 2^(4k+3)       */

    /* Base power of 2: 2^(4k+3) */
    seq_t pow = pow_seq((seq_t)2, exp);

    switch (r) {
        case 4:                       /* a(8k+4) = 2^(4k+3)         */
        case 7:                       /* a(8k+7) = 2^(4k+3)         */
            return pow;

        case 5:                       /* a(8k+5) = 5*2^(4k+2)      */
            return (pow_seq((seq_t)2, exp - 1)) * (seq_t)5;

        case 6:                       /* a(8k+6) = 3*2^(4k+3)      */
            return pow * (seq_t)3;

        default:                        /* Should never reach here   */
            return 0;
    }
}
