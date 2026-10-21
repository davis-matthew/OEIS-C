/*
 * A216454  G.f. satisfies: A(x) = (1 + x*(3-x)*A(x)) * (1 + x^2*A(x)).
 * https://oeis.org/A216454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0)=1, a(1)=3, a(2)=9, a(3)=30, a(4)=107
   3*(n+3)*a(n) = (19*n+30)*a(n-1)
                 - 3*(11*n+3)*a(n-2)
                 + 9*(5*n-6)*a(n-3)
                 - 6*(4*n-9)*a(n-4)
                 + 4*(n-3)*a(n-5)   for n ≥ 5
*/
#include <stdint.h>

seq_t a(idx_t n)
{
    static const seq_t init[5] = {
        (seq_t)1, (seq_t)3, (seq_t)9, (seq_t)30, (seq_t)107
    };
    if (n < 5)
        return init[n];

    seq_t buf[5];
    for (int j = 0; j < 5; ++j) buf[j] = init[j];

    for (idx_t i = 5; i <= n; ++i) {
        seq_t n1 = buf[4];
        seq_t n2 = buf[3];
        seq_t n3 = buf[2];
        seq_t n4 = buf[1];
        seq_t n5 = buf[0];
        seq_t res;

        if (sizeof(seq_t) <= 8) {             /* 8‑byte or smaller */
            unsigned __int128 num = 0;
            unsigned __int128 coeff;

            coeff = (unsigned __int128)19 * (unsigned __int128)i + (unsigned __int128)30;
            num += coeff * (unsigned __int128)n1;

            coeff = (unsigned __int128)3 * ((unsigned __int128)11 * (unsigned __int128)i + (unsigned __int128)3);
            num -= coeff * (unsigned __int128)n2;

            coeff = (unsigned __int128)9 * ((unsigned __int128)5 * (unsigned __int128)i - (unsigned __int128)6);
            num += coeff * (unsigned __int128)n3;

            coeff = (unsigned __int128)6 * ((unsigned __int128)4 * (unsigned __int128)i - (unsigned __int128)9);
            num -= coeff * (unsigned __int128)n4;

            coeff = (unsigned __int128)4 * ((unsigned __int128)i - (unsigned __int128)3);
            num += coeff * (unsigned __int128)n5;

            unsigned __int128 denom = (unsigned __int128)3 * ((unsigned __int128)i + (unsigned __int128)3);
            num /= denom;

            res = (seq_t)num;
        } else {                               /* arbitrary‑precision type */
            seq_t num = 0;

            num += (seq_t)(((seq_t)19 * (seq_t)i) + (seq_t)30) * n1;
            num -= (seq_t)((seq_t)3 * ((seq_t)11 * (seq_t)i + (seq_t)3)) * n2;
            num += (seq_t)((seq_t)9 * ((seq_t)5 * (seq_t)i - (seq_t)6)) * n3;
            num -= (seq_t)((seq_t)6 * ((seq_t)4 * (seq_t)i - (seq_t)9)) * n4;
            num += (seq_t)((seq_t)4 * ((seq_t)i - (seq_t)3)) * n5;

            seq_t denom = (seq_t)((seq_t)3 * ((seq_t)i + (seq_t)3));
            res = num / denom;
        }

        /* shift buffer */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = res;
    }
    return buf[4];
}
