/*
 * A235593  Binomial(n-1,3)+3*binomial(n-1,4)+6*binomial(n-1,5)+5*binomial(n-1,6).
 * https://oeis.org/A235593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            binomial(n-1,3)+3*binomial(n-1,4)+6*binomial(n-1,5)+5*binomial(n-1,6)
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   binomial, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n < 4) return (seq_t)0;

    seq_t res = binomial(n - 1, 3);

    if (n >= 5) {
        seq_t t = binomial(n - 1, 4);
        res += mul_small(t, 3);
    }
    if (n >= 6) {
        seq_t t = binomial(n - 1, 5);
        res += mul_small(t, 6);
    }
    if (n >= 7) {
        seq_t t = binomial(n - 1, 6);
        res += mul_small(t, 5);
    }

    return res;
}
