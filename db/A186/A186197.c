/*
 * A186197  Number of (n+1)X2 0..3 arrays with no 2X2 subblock diagonal sum
 *          less antidiagonal sum equal to any horizontal or vertical neighbor
 *          2X2 subblock diagonal sum less antidiagonal sum.
 * https://oeis.org/A186197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            6*a(n-1)+71*a(n-2)+539*a(n-3)+2454*a(n-4)+7868*a(n-5)+8056*a(n-6)
 *            -32420*a(n-7)-165096*a(n-8)-411360*a(n-9)-936288*a(n-10)
 *            -1586880*a(n-11)-1264896*a(n-12)-331776*a(n-13) Initial values:
 *            a(1)=256, a(2)=3692, a(3)=53462, a(4)=773776, a(5)=11199932,
 *            a(6)=162110182, a(7)=2346415828, a(8)=3396...
 * Range:     u8 none, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t init[14] = {
    0ULL,                      /* unused; indices start at 1 */
    256ULL, 3692ULL, 53462ULL, 773776ULL, 11199932ULL,
    162110182ULL, 2346415828ULL, 33962540254ULL, 491581127962ULL,
    7115251677062ULL, 102987693303636ULL, 1490666163351236ULL,
    21576224718636722ULL
};

static const long long coeffs[13] = {
    6LL, 71LL, 539LL, 2454LL, 7868LL, 8056LL,
   -32420LL, -165096LL, -411360LL, -936288LL,
   -1586880LL, -1264896LL, -331776LL
};

seq_t a(idx_t n)
{
    if (n <= 13)
        return init[(size_t)n];

    /* prev[0] = a(k-1), prev[1] = a(k-2), ..., prev[12] = a(k-13) */
    seq_t prev[13];
    for (int i = 0; i < 13; ++i)
        prev[i] = init[13 - i];

    seq_t new_val = 0;
    for (idx_t k = 14; k <= n; ++k) {
        seq_t sum = 0;
        for (int t = 0; t < 13; ++t) {
            seq_t term = prev[t];
            if (coeffs[t] >= 0)
                sum += (seq_t)coeffs[t] * term;
            else
                sum -= (seq_t)(-coeffs[t]) * term;
        }
        new_val = sum;

        /* shift prev: a(k) becomes next a(k-1) */
        for (int i = 12; i > 0; --i)
            prev[i] = prev[i-1];
        prev[0] = new_val;
    }
    return new_val;
}
