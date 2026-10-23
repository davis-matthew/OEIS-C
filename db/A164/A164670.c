/*
 * A164670  Number of reduced words of length n in Coxeter group on 34
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^7 = I.
 * https://oeis.org/A164670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 32*(a(n-1)+...+a(n-6)) - 528*a(n-7) with a(0..6) =
 *            1,34,1122,37026,1221858,40321314,1330603362 (Note: a(7) requires
 *            an extra +1 due to the numerator of the generating function; all
 *            subsequent terms follow the recurrence)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* initial values a(0)..a(6) */
    static const seq_t init[7] = {
        (seq_t)1ULL,
        (seq_t)34ULL,
        (seq_t)1122ULL,
        (seq_t)37026ULL,
        (seq_t)1221858ULL,
        (seq_t)40321314ULL,
        (seq_t)1330603362ULL
    };

    if (n < 7)
        return init[(int)n];

    /* circular buffer holding a(n-7)..a(n-1) */
    seq_t buf[7];
    for (int i = 0; i < 7; ++i)
        buf[i] = init[i];

    for (idx_t pos = 7; pos <= n; ++pos) {
        /* sum of the previous six terms */
        seq_t sum = buf[(int)((pos - 1) % 7)] +
                    buf[(int)((pos - 2) % 7)] +
                    buf[(int)((pos - 3) % 7)] +
                    buf[(int)((pos - 4) % 7)] +
                    buf[(int)((pos - 5) % 7)] +
                    buf[(int)((pos - 6) % 7)];

        /* multiply by 32 */
        sum = mul_small(sum, 32);

        /* subtract 528*a(pos-7) */
        sum -= mul_small(buf[(int)((pos - 7) % 7)], 528);

        /* for n==7 add the contribution from the numerator (coeff 1) */
        if (pos == 7)
            sum += (seq_t)1;

        buf[(int)(pos % 7)] = sum;
    }

    return buf[(int)(n % 7)];
}
