/*
 * A134325  a(n) = a(n) = +13*a(n-2) +7*a(n-3) -47*a(n-4) -38*a(n-5)
 *          +48*a(n-6) +43*a(n-7) -7*a(n-8) -1*a(n-9).
 * https://oeis.org/A134325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(n) =
 *            +13*a(n-2)+7*a(n-3)-47*a(n-4)-38*a(n-5)+48*a(n-6)+43*a(n-7)-7*a(n-8)-1*a(n-9)
 *            Initial values (offset 1): a(1)=3 , a(2)=9 , a(3)=29 , a(4)=87 ,
 *            a(5)=281 , a(6)=857 , a(7)=2741 , a(8)=8471 , a(9)=26876
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Initial values a(1) … a(9).  init[0] is unused. */
    static const seq_t init[10] = {
        0,          /* unused for a(0) */
        3,          /* a(1) */
        9,          /* a(2) */
        29,         /* a(3) */
        87,         /* a(4) */
        281,        /* a(5) */
        857,        /* a(6) */
        2741,       /* a(7) */
        8471,       /* a(8) */
        26876       /* a(9) */
    };

    /* The offset is 1; a(1) is the first supplied term. */
    if (n < 1)
        return (seq_t)0;          /* undefined for n < 1 */

    if (n <= 9)
        return init[n];

    /* Sliding window of the last nine terms:
       prev[0] = a(k-9), … , prev[8] = a(k-1).            */
    seq_t prev[9];
    for (idx_t i = 0; i < 9; ++i)
        prev[i] = init[i + 1];

    seq_t val;
    for (idx_t k = 10; k <= n; ++k) {
        /* Compute the recurrence using unsigned arithmetic
           avoiding signed overflow.  Separate positive and
           negative contributions. */
        seq_t pos = (prev[7] * (seq_t)13) +
                     (prev[6] * (seq_t)7)  +
                     (prev[3] * (seq_t)48) +
                     (prev[2] * (seq_t)43);

        seq_t neg = (prev[5] * (seq_t)47) +
                     (prev[4] * (seq_t)38) +
                     (prev[1] * (seq_t)7)  +
                     prev[0];

        val = pos - neg;   /* final term a(k) */

        /* Shift the window: discard the oldest term, shift left,
           and insert the new value at the end. */
        for (idx_t i = 0; i < 8; ++i)
            prev[i] = prev[i + 1];
        prev[8] = val;
    }

    return prev[8];   /* a(n) */
}
