/*
 * A076785  Let u(1)=u(2)=u(3)=1,
 *          u(n)=(2^floor(u(n-1)/2)+2^floor(u(n-2)/2)+1)/u(n-3) then a(n) =
 *          denominator of u(n).
 * https://oeis.org/A076785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = denominator of u(n) where u(1)=u(2)=u(3)=1, u(n) =
 *            (2^⌊u(n-1)/2⌋ + 2^⌊u(n-2)/2⌋ + 1) / u(n-3)
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   gcd, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/pow_seq.h"

typedef struct { seq_t num; seq_t den; } rat_t;

seq_t a(idx_t n)
{
    if (n <= 3) return 1;

    rat_t t1 = {1, 1}; /* u(n-3) */
    rat_t t2 = {1, 1}; /* u(n-2) */
    rat_t t3 = {1, 1}; /* u(n-1) */

    for (idx_t i = 4; i <= n; ++i)
    {
        /* floor(u/2) as integer division */
        seq_t fl1 = t3.num / ( (seq_t)2 * t3.den );
        seq_t fl2 = t2.num / ( (seq_t)2 * t2.den );

        int exp1 = (int)fl1;
        int exp2 = (int)fl2;

        seq_t pw1 = pow_seq((seq_t)2, exp1);
        seq_t pw2 = pow_seq((seq_t)2, exp2);

        seq_t num_new = pw1 + pw2 + 1;
        seq_t num_res = num_new * t1.den;
        seq_t den_res = t1.num;

        seq_t g = gcd(num_res, den_res);
        num_res /= g;
        den_res /= g;

        /* shift for next iteration */
        t1 = t2;
        t2 = t3;
        t3 = (rat_t){num_res, den_res};
    }

    return t3.den;
}
