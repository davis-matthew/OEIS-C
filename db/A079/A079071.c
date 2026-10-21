/*
 * A079071  Number of numbers < n whose binary representation has the same
 *          number of 0's and 1's as n does.
 * https://oeis.org/A079071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = # of m < n whose binary representation contains the same
 *            number of 1's and the same length as that of n
 * Range:     u8 n <= 3101, u16 n <= 9718, u32 n <= 9718, u64 n <= 9718,
 *            u128 n <= 9718, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* binomial coefficient C(n, k) for 0 <= n, k <= 64 */
static inline seq_t binom(idx_t n, int k)
{
    if (k < 0 || (idx_t)k > n) return 0;
    if (k > (int)(n - k)) k = (int)(n - k);
    seq_t res = 1;
    for (int i = 1; i <= k; ++i) {
        /* res = res * (n - k + i) / i  */
        __int128 tmp = (__int128)res * (n - (idx_t)k + (idx_t)i);
        res = (seq_t)(tmp / i);
    }
    return res;
}

/* floor(log2(x)) for x > 0 */
static inline idx_t floor_log2_uint64(idx_t x)
{
    idx_t r = 0;
    while (x >> 1) { x >>= 1; ++r; }
    return r;
}

/* Count numbers <= upper with exactly k ones in binary */
static seq_t count_pop_leq(idx_t upper, int k)
{
    if (upper == (idx_t)-1) return 0;    /* empty range */
    seq_t cnt = 0;
    int ones = 0;
    int msb = floor_log2_uint64(upper);
    for (int pos = msb; pos >= 0; --pos) {
        if (upper & ((idx_t)1 << pos)) {
            int need = k - ones;
            if (need >= 0 && (idx_t)need <= (idx_t)pos)
                cnt += binom((idx_t)pos, need);
            ++ones;
        }
    }
    if (ones == k) ++cnt;   /* include 'upper' itself if it has k ones */
    return cnt;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* a(0) = 0 */

    int k = __builtin_popcountll(n);    /* number of 1's in n */

    int len = 1 + floor_log2_uint64(n); /* length of binary representation of n */
    idx_t low = ((idx_t)1) << (len - 1);/* smallest number with this length */

    if (n <= low) return 0;              /* no m with same length < n */

    idx_t high = n - 1;                  /* upper bound of the interval */

    seq_t cnt_high = count_pop_leq(high, k);
    seq_t cnt_low_minus = count_pop_leq(low - 1, k);
    return cnt_high - cnt_low_minus;
}
