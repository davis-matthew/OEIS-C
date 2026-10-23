/*
 * A124441  a(n) = Product_{1<=k<=n/2, gcd(k,n)=1} k.
 * https://oeis.org/A124441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{1<=k<=n/2, gcd(k,n)=1} k
 * Range:     u8 n <= 12, u16 n <= 18, u32 n <= 28, u64 n <= 42, u128 n <= 70,
 *            big any n
 * Helpers:   gcd, seq_mul, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/seq_one.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* a(1) = 1 and for n>1 the product starts at 1 and goes up to floor(n/2). */
    seq_t prod = seq_one();          /* initialise product to 1 */
    idx_t half = n / 2;              /* floor(n/2) */

    for (idx_t k = 1; k <= half; ++k)
    {
        if (gcd((seq_t)k, (seq_t)n) == seq_one())
        {
            prod = seq_mul(prod, (seq_t)k);
        }
    }
    return prod;
}
