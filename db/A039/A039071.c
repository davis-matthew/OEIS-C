/*
 * A039071  Numbers whose base-8 representation has the same number of 4's and
 *          5's.
 * https://oeis.org/A039071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 0; for n >= 2 the nth positive integer x * such that in
 *            base‑8 the number of 4’s equals the number of 5’s. * The
 *            sequence is found by combinatorial ranking
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* static tables for binomial coefficients and powers of 6   */
static bool tables_ready = false;
static uint64_t comb[65][65];
static uint64_t pow6[65];

static void init_tables(void) {
    if (tables_ready) return;
    tables_ready = true;
    /* binomial coefficients 0 <= n <= 64 */
    for (int n = 0; n <= 64; ++n) {
        comb[n][0] = comb[n][n] = 1ULL;
        for (int k = 1; k < n; ++k) {
            __uint128_t tmp = (__uint128_t)comb[n-1][k-1] + comb[n-1][k];
            comb[n][k] = (uint64_t)tmp;
        }
    }
    /* powers of 6 */
    pow6[0] = 1ULL;
    for (int i = 1; i <= 64; ++i) {
        __uint128_t tmp = (__uint128_t)pow6[i-1] * 6;
        pow6[i] = (uint64_t)tmp;
    }
}

/* number of balanced completions for a prefix of length pos
 * in a number of total length L, having already cnt4 4's and cnt5 5's   */
static uint64_t count_completions(idx_t L, idx_t pos, uint64_t cnt4, uint64_t cnt5) {
    uint64_t r = L - pos;                      /* remaining free positions   */
    int64_t  diff = (int64_t)cnt4 - (int64_t)cnt5;   /* current difference 4-5      */

    if (r == 0) {                              /* end of number              */
        return (cnt4 == cnt5) ? 1ULL : 0ULL;
    }

    uint64_t lower = (diff >= 0) ? 0ULL : (uint64_t)(-diff);
    int64_t   diff_i  = diff;
    int64_t   upper_i = ((int64_t)r - diff_i) / 2;

    if (upper_i < (int64_t)lower) return 0ULL;

    uint64_t total = 0ULL;
    for (uint64_t x = lower; x <= (uint64_t)upper_i; ++x) {
        int64_t y = (int64_t)x + diff_i;
        if (y < 0 || (int64_t)x + y > (int64_t)r) continue;
        uint64_t c1 = comb[r][x];
        uint64_t c2 = comb[r - x][y];
        uint64_t p6 = pow6[r - x - y];
        __uint128_t prod = (__uint128_t)c1 * c2 * p6;
        total += (uint64_t)prod;
    }
    return total;
}

seq_t a(idx_t n) {
    /* The first element is 0, so adjust for it */
    if (n == 1) return (seq_t)0ULL;
    idx_t m = n - 1;                /* index among positive numbers    */
    init_tables();

    uint64_t cumulative = 0ULL;
    idx_t    L = 1;                 /* current length being inspected  */

    for (;; ++L) {
        uint64_t count_len = 0ULL;

        for (int d = 1; d <= 7; ++d) {
            uint64_t cnt4 = (d == 4) ? 1 : 0;
            uint64_t cnt5 = (d == 5) ? 1 : 0;
            count_len += count_completions(L, 1, cnt4, cnt5);
            if (count_len > UINT64_MAX - cumulative) {
                /* overflow guard – should not happen for required n */
                return (seq_t)0ULL;
            }
        }

        if (cumulative + count_len >= (uint64_t)m) break;
        cumulative += count_len;
    }

    uint64_t offset = (uint64_t)m - cumulative;     /* 1‑based offset in this length */
    uint64_t val = 0ULL;
    uint64_t cnt4 = 0ULL, cnt5 = 0ULL;

    for (idx_t pos = 0; pos < L; ++pos) {
        int start = (pos == 0) ? 1 : 0;
        for (int d = start; d <= 7; ++d) {
            uint64_t n_cnt4 = cnt4 + (d == 4);
            uint64_t n_cnt5 = cnt5 + (d == 5);
            uint64_t completions = count_completions(L, pos + 1, n_cnt4, n_cnt5);

            if (offset > completions) {
                offset -= completions;
                continue;
            }

            val = val * 8ULL + (uint64_t)d;
            cnt4 = n_cnt4;
            cnt5 = n_cnt5;
            break;
        }
    }

    return (seq_t)val;
}
