/*
 * A045273  Numbers whose base-5 representation contains exactly two 2's and
 *          three 3's.
 * https://oeis.org/A045273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base-5 representation contains exactly two 2's and three 3's.
   The function enumerates the numbers in increasing order using rank‑based
   construction. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

static uint64_t comb_table[25][25] = {0};

static void init_combinations(void) {
    for (size_t n = 0; n < 25; ++n) {
        comb_table[n][0] = comb_table[n][n] = 1;
        for (size_t k = 1; k < n; ++k) {
            comb_table[n][k] = comb_table[n - 1][k - 1] + comb_table[n - 1][k];
        }
    }
}

static uint64_t comb(uint64_t n, uint64_t k) {
    if (k > n) return 0;
    return comb_table[n][k];
}

/* power of 3 up to exponent 20 */
static uint64_t pow3[21];

static void init_pow3(void) {
    pow3[0] = 1;
    for (size_t i = 1; i < 21; ++i) pow3[i] = pow3[i - 1] * 3ULL;
}

static uint64_t count_completions(size_t len_rem, int rem2, int rem3) {
    if (rem2 < 0 || rem3 < 0) return 0;
    int need = rem2 + rem3;
    if ((size_t)need > len_rem) return 0;
    uint64_t res = comb(len_rem, need);
    res *= comb(need, rem2);
    res *= pow3[len_rem - (size_t)need];
    return res;
}

static void init_metadata(void);

static size_t init_done = 0;
static uint64_t total_cnt[25] = {0};   /* total strings of length L (ignoring leading zero) */
static uint64_t valid_cnt[25] = {0};   /* strings with leading digit != 0 */
static uint64_t cum_cnt[25]   = {0};   /* cumulative up to length L */

static void init_metadata(void) {
    if (init_done) return;
    init_combinations();
    init_pow3();
    /* compute for lengths 0..20 (we only need up to 20 for safety) */
    for (size_t L = 5; L < 21; ++L) {
        uint64_t t = comb(L, 5) * 10ULL * pow3[L - 5];
        total_cnt[L] = t;
    }
    valid_cnt[5] = total_cnt[5];
    for (size_t L = 6; L < 21; ++L) {
        valid_cnt[L] = total_cnt[L] - total_cnt[L - 1];
    }
    cum_cnt[4] = 0;
    for (size_t L = 5; L < 21; ++L) {
        cum_cnt[L] = cum_cnt[L - 1] + valid_cnt[L];
    }
    init_done = 1;
}

seq_t a(idx_t n) {
    init_metadata();
    /* Find length */
    size_t L = 5;
    while (L < 21 && cum_cnt[L] < (uint64_t)n) ++L;
    if (L >= 21) return (seq_t)0; /* out of range; shouldn't happen for n<=10000 */

    uint64_t rank = (uint64_t)n - cum_cnt[L - 1]; /* rank within this length block (1-indexed) */

    int rem2 = 2;
    int rem3 = 3;
    size_t rem_len = L;
    seq_t value = 0;

    for (size_t pos = 0; pos < L; ++pos) {
        int start = (pos == 0) ? 1 : 0;
        int chosen = -1;
        for (int d = start; d <= 4; ++d) {
            int new2 = rem2 - (d == 2);
            int new3 = rem3 - (d == 3);
            uint64_t cnt = count_completions(rem_len - 1, new2, new3);
            if (rank <= cnt) {
                chosen = d;
                rem2 = new2;
                rem3 = new3;
                break;
            }
            rank -= cnt;
        }
        /* Append chosen digit */
        value = value * 5 + (seq_t)chosen;
        rem_len -= 1;
    }
    return value;
}
