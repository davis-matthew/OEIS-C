/*
 * A102090  Number of matchings in the C_n X P_3 graph (C_n is the cycle graph
 *          on n vertices and P_3 is the path graph on 3 vertices).
 * https://oeis.org/A102090
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 19*a(n-2) + 10*a(n-3) - 24*a(n-4) - 10*a(n-5)
 *            + 11*a(n-6) + a(n-7) - a(n-8) with initial values a(2)=47,
 *            a(3)=228, a(4)=1511, a(5)=9213, a(6)=57536, a(7)=356863,
 *            a(8)=2217871, a(9)=13775700 (a(0) and a(1) are unused)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t memo[10001];
static idx_t memo_calculated = 1;  /* highest index computed so far */

static void init_memo(void)
{
    if (memo_calculated > 1) return;   /* already initialized */

    memo[0] = 0;
    memo[1] = 0;
    memo[2] = (seq_t)47;
    memo[3] = (seq_t)228;
    memo[4] = (seq_t)1511;
    memo[5] = (seq_t)9213;
    memo[6] = (seq_t)57536;
    memo[7] = (seq_t)356863;
    memo[8] = (seq_t)2217871;
    memo[9] = (seq_t)13775700;
    memo_calculated = 9;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;           /* values below the offset are undefined */

    init_memo();

    if (n <= memo_calculated)
        return memo[n];

    /* compute missing values forward using the recurrence */
    for (idx_t i = memo_calculated + 1; i <= n; ++i) {
        seq_t val = 0;
        val = 3 * memo[i - 1] + 19 * memo[i - 2] + 10 * memo[i - 3];
        val = val - 24 * memo[i - 4] - 10 * memo[i - 5]
                 + 11 * memo[i - 6] + memo[i - 7] - memo[i - 8];
        memo[i] = val;
    }
    memo_calculated = n;
    return memo[n];
}
