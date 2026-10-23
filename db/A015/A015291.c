/*
 * A015291  Gaussian binomial coefficient [ n,4 ] for q = -5.
 * https://oeis.org/A015291
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Gaussian binomial coefficient [n,4] for q = -5 (sequence
 *            A015291)
 * Range:     u8 none, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 4) return 0;                /* values below the offset are undefined */

    const seq_t primes[4] = {2, 3, 7, 13};
    int remaining[4] = {9, 5, 1, 1};    /* exponents in the denominator 2^9 * 3^5 * 7 * 13 */

    seq_t powCurr = big_pow((seq_t)5, (seq_t)n);   /* 5^n  */
    seq_t result  = 1;

    for (idx_t i = 0; i < 4; ++i) {
        seq_t term;
        /* compute |1 - (-5)^{n-i}| : 5^(n-i) ± 1  */
        if (((n - i) & 1) == 0)   /* even exponent -> 1 - 5^(n-i)  */
            term = powCurr - 1;
        else                       /* odd exponent -> 1 + 5^(n-i)   */
            term = powCurr + 1;

        /* cancel factors of the denominator from this term */
        for (int p = 0; p < 4; ++p) {
            while (remaining[p] > 0 && (term % primes[p]) == 0) {
                term /= primes[p];
                --remaining[p];
            }
        }

        result *= term;
        powCurr /= 5;             /* next power: 5^(n-i-1)  */
    }

    /* if any denominator factors remain, cancel them from the product */
    for (int p = 0; p < 4; ++p) {
        while (remaining[p] > 0) {
            result /= primes[p];
            --remaining[p];
        }
    }

    return result;
}
