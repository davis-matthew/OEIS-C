/*
 * A123555  Number of standard Young tableaux of type (n+1,n,n-1).
 * https://oeis.org/A123555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 16*(3*n)!/((n-1)!*(n+1)!*(n+3)!)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 30,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent recurrence: a(0)=0, a(1)=2,
   (n-1)*(n+3)*(n+1)*a(n) = 3*n*(3*n-1)*(3*n-2)*a(n-1)  for n>1. */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Handle the special initial terms */
    if (n == 0)
        return 0;          /* a(0) = 0 */
    if (n == 1)
        return 2;          /* a(1) = 2 */

    seq_t val = 2;         /* start from a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        /* Denominator of the recurrence ratio: (i-1)*(i+1)*(i+3) */
        seq_t denom = (seq_t)(i - 1) * (seq_t)(i + 1) * (seq_t)(i + 3);

        /* Reduce common factors between current value and denominator */
        seq_t g = gcd(val, denom);
        if (g > 1) {
            val   /= g;
            denom /= g;
        }

        /* Numerators: 3*i, 3*i-1, 3*i-2 */
        const unsigned long long nums[3] = { 3 * i, 3 * i - 1, 3 * i - 2 };

        for (int k = 0; k < 3; ++k) {
            seq_t num = (seq_t)nums[k];

            /* Cancel as many factors of the denominator from this numerator */
            seq_t g2 = gcd(num, denom);
            if (g2 > 1) {
                denom /= g2;
                num /= g2;
            }

            /* Multiply the reduced numerator into the running product */
            val *= num;
        }

        /* The recurrence guarantees that any remaining denominator
           divides the product exactly.  If something remained, divide it. */
        if (denom != 1)
            val /= denom;
    }

    return val;
}
