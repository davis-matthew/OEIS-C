/*
 * A140926  Number of 3 X 10 matrices with elements in 0..n with each row and
 *          each column in nondecreasing order. 3,10,n can be permuted, see
 *          formula.
 * https://oeis.org/A140926
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i=0}^{2} ( binomial(n+12, n+i) * factorial(i) /
 *            (n+12-i)^(2-i) ) (with p=n, q=10, r=3 and s=p+q+r-1)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   binomial, factorial, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/factorial.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* parameters of the formula */
    const idx_t p = n;          /* p = n               */
    const idx_t q = 10;         /* q = 10              */
    const idx_t r = 3;          /* r = 3               */
    const idx_t s = p + q + r - 1;   /* s = n + 12 */

    /* use a 128‑bit temporary for the intermediate products */
    unsigned __int128 prod = 1;   /* numerator product    */
    unsigned __int128 den  = 1;   /* denominator product  */

    for (idx_t i = 0; i < r; ++i)
    {
        /* binomial(s, p+i)  *  i!   */
        seq_t bin  = binomial(s, p + i);
        seq_t fact = factorial(i);
        unsigned __int128 term = (unsigned __int128)bin * (unsigned __int128)fact;
        prod *= term;

        /* (s-i)^(r-i-1)   */
        idx_t exp = r - i - 1;     /* 2, 1, 0 for i=0..2 */
        if (exp > 0)
        {
            seq_t base = (seq_t)(s - i);
            seq_t pv   = pow_seq(base, (int)exp);
            den *= (unsigned __int128)pv;
        }
    }

    /* the division is exact */
    unsigned __int128 res = prod / den;
    return (seq_t)res;
}
