/*
 * A183878  Number of arrangements of n+2 numbers in 0..3 with each number
 *          being the sum mod 4 of two others.
 * https://oeis.org/A183878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n-1) - 58*a(n-2) + 148*a(n-3) - 217*a(n-4) +
 *            184*a(n-5) - 84*a(n-6) + 16*a(n-7) (for n > 9) with a(1)=4,
 *            a(2)=35, a(3)=446, a(4)=2827, a(5)=13686, a(6)=59859,
 *            a(7)=250198, a(8)=1023347, a(9)=4140830
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mul_small.h"   /* provides mul_small(seq_t, unsigned int) */

seq_t a(idx_t n)
{
    /* Base values for n = 1..9 */
    static const seq_t base[9] = {
        (seq_t)4,
        (seq_t)35,
        (seq_t)446,
        (seq_t)2827,
        (seq_t)13686,
        (seq_t)59859,
        (seq_t)250198,
        (seq_t)1023347,
        (seq_t)4140830
    };

    /* Small static cache large enough for all requests (n <= 10000) */
    enum { MAX_N = 10010 };
    static int initialized = 0;
    static seq_t cache[MAX_N];
    static idx_t computed = 0;

    if (!initialized) {
        /* initialise base values */
        for (idx_t i = 1; i <= 9; ++i)
            cache[i - 1] = base[i - 1];
        computed = 9;
        initialized = 1;
    }

    if (n <= 9)
        return cache[n - 1];

    if (n > computed) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            /* recurrence uses cache indexed from 0 */
            seq_t v1 = mul_small(cache[i - 2], 12);
            seq_t v2 = mul_small(cache[i - 3], 58);
            seq_t v3 = mul_small(cache[i - 4], 148);
            seq_t v4 = mul_small(cache[i - 5], 217);
            seq_t v5 = mul_small(cache[i - 6], 184);
            seq_t v6 = mul_small(cache[i - 7], 84);
            seq_t v7 = mul_small(cache[i - 8], 16);
            cache[i - 1] = v1 - v2 + v3 - v4 + v5 - v6 + v7;
        }
        computed = n;
    }

    return cache[n - 1];
}
