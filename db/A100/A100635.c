/*
 * A100635  Number of 2 X 2 matrices with elements in {1,2,...,n} such that
 *          LCMs of rows and columns are n.
 * https://oeis.org/A100635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, easy, nonn
 * Formula:   a(n) = ∏_{p^e∥n} ( 2·(e+1)² – 1 ) (multiplicative)
 * Range:     u8 n <= 29, u16 n <= 9239, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t x = n;
    seq_t result = 1;

    /* handle factor 2 */
    if ((x & 1) == 0) {
        unsigned int exp = 0;
        while ((x & 1) == 0) {
            exp++;
            x >>= 1;
        }
        seq_t t = ((seq_t)(exp + 1) * (exp + 1)) * 2 - 1;
        result *= t;
    }

    /* odd factors */
    for (idx_t p = 3; p <= x / p; p += 2) {
        if (x % p == 0) {
            unsigned int exp = 0;
            while (x % p == 0) {
                exp++;
                x /= p;
            }
            seq_t t = ((seq_t)(exp + 1) * (exp + 1)) * 2 - 1;
            result *= t;
        }
    }

    /* remaining prime factor (if any) */
    if (x > 1) {
        seq_t t = ((seq_t)2 * (seq_t)2) * 2 - 1;   /* exp = 1 → (1+1)^2 = 4 */
        result *= t;
    }

    return result;
}
