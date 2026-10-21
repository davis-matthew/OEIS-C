/*
 * A217694  Number of n-variations of the set {1,2,...,n+1} satisfying p(i)-i
 *          in {-2,0,2}, i=1..n (an n-variation of the set N_{n+s} =
 *          {1,2,...,n+s} is any 1-to-1 mapping p from the set N_n =
 *          {1,2,...,n} into N_{n+s} = {1,2,...,n+s}).
 * https://oeis.org/A217694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)+2*a(n-4)-2*a(n-5)-a(n-6)-a(n-7)-a(n-8)
 *            Initial values: a(0)=1, a(1)=1, a(2)=2, a(3)=4, a(4)=8, a(5)=12,
 *            a(6)=21, a(7)=35, a(8)=60
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    /* Base values for n = 0..8 */
    static const seq_t base[9] = {
        (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)4, (seq_t)8,
        (seq_t)12, (seq_t)21, (seq_t)35, (seq_t)60
    };

    if (n <= 8) {
        return base[n];
    }

    /* Rolling buffer of last 9 values */
    seq_t buf[9];
    for (size_t i = 0; i < 9; ++i) buf[i] = base[i];

    /* Compute values iteratively using the recurrence */
    for (idx_t cur = 9; cur <= n; ++cur) {
        seq_t v = buf[(size_t)(cur - 1) % 9]
                + buf[(size_t)(cur - 2) % 9]
                + (seq_t)2 * buf[(size_t)(cur - 4) % 9];
        v -= (seq_t)2 * buf[(size_t)(cur - 5) % 9];
        v -= buf[(size_t)(cur - 6) % 9];
        v -= buf[(size_t)(cur - 7) % 9];
        v -= buf[(size_t)(cur - 8) % 9];
        buf[(size_t)cur % 9] = v;
    }

    return buf[(size_t)n % 9];
}
