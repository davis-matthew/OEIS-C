/*
 * A081666  n*3^(n-1)+A081567(n).
 * https://oeis.org/A081666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 11*a(n-1) - 44*a(n-2) + 75*a(n-3) - 45*a(n-4) for n ≥ 4,
 *            with a(0) = 1, a(1) = 4, a(2) = 16, a(3) = 62
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#ifndef MAX_N
#define MAX_N 10000
#endif

static seq_t memo[MAX_N + 1];
static size_t memo_next = 4;     /* next index to compute   */
static int memo_inited = 0;     /* flag: 0 → not initialized yet */

static void memo_init(void)
{
    if (!memo_inited) {
        memo[0] = (seq_t)1;
        memo[1] = (seq_t)4;
        memo[2] = (seq_t)16;
        memo[3] = (seq_t)62;
        memo_inited = 1;        /* initialization done   */
    }
}

seq_t a(idx_t n)
{
    memo_init();

    size_t ni = (size_t)n;
    if (ni < memo_next)            /* already computed  */
        return memo[ni];

    for (size_t i = memo_next; i <= ni; ++i) {
        seq_t term1 = (seq_t)11 * memo[i - 1];
        seq_t term3 = (seq_t)75 * memo[i - 3];
        seq_t sum1  = term1 + term3;

        seq_t term2 = (seq_t)44 * memo[i - 2];
        seq_t term4 = (seq_t)45 * memo[i - 4];
        seq_t sum2  = term2 + term4;

        memo[i] = sum1 - sum2;    /* sum1 ≥ sum2 for all i ≥ 4 */
    }
    memo_next = ni + 1;
    return memo[ni];
}
