/*
 * A091669  a(n) = (2^(n-1)/n!) * Product_{k=1..n-1} (2^k-1).
 * https://oeis.org/A091669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{k=2..n} (2^k – 2) / k (a(1) = 1)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t if needed */

static seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; for n < 2 we return 1. */
    if (n < 2)
        return (seq_t)1;

    seq_t result = (seq_t)1;   /* running product of the reduced factors */
    seq_t pow2   = (seq_t)2;   /* holds 2^k, starting at 2^1 */

    for (idx_t k = 2; k <= n; ++k) {
        /* Advance to 2^k */
        pow2 <<= 1;                     /* pow2 = 2^k */

        /* Numerator = 2^k – 2 */
        seq_t num = pow2 - (seq_t)2;

        /* Denominator = k */
        seq_t den = (seq_t)k;

        /* Cancel common factor between den and accumulated result. */
        seq_t g = gcd_seq_t(den, result);
        if (g > (seq_t)1) {
            den   /= g;
            result /= g;
        }

        /* Cancel common factor between num and remaining den. */
        g = gcd_seq_t(num, den);
        if (g > (seq_t)1) {
            num /= g;
            den /= g;
        }

        /* Multiply by remaining numerator and divide by remaining denominator. */
        result *= num;
        if (den > (seq_t)1)
            result /= den;
    }

    return result;
}
