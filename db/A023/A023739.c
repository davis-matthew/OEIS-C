/*
 * A023739  Numbers with exactly 2 4's in base 5 expansion.
 * https://oeis.org/A023739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n-th integer whose base‑5 representation contains
 *            exactly two 4's
 * Range:     u8 n <= 24, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static void init_pow4(void);
static seq_t comb_int(int n, int k);
static seq_t count_len(int L);
static seq_t count_ways(int positions, int needed);
static seq_t pow4[128];
static int pow4_inited = 0;

static void init_pow4(void)
{
    if (!pow4_inited) {
        pow4[0] = 1;
        for (int i = 1; i < 128; ++i)
            pow4[i] = pow4[i - 1] * (seq_t)4;
        pow4_inited = 1;
    }
}

static seq_t comb_int(int n, int k)
{
    if (k < 0 || k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (int i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;  /* exact division */
    }
    return res;
}

static seq_t count_len(int L)
{
    if (L < 2) return 0;
    init_pow4();
    seq_t res1 = (seq_t)(L - 1) * pow4[L - 2];          /* first digit 4 */
    seq_t res2 = 0;
    if (L - 1 >= 2) {
        res2 = 3 * comb_int(L - 1, 2) * pow4[L - 3];   /* first digit 1–3 */
    }
    return res1 + res2;
}

static seq_t count_ways(int positions, int needed)
{
    if (needed < 0 || needed > positions) return 0;
    init_pow4();
    seq_t c = comb_int(positions, needed);
    seq_t p = pow4[positions - needed];
    return c * p;
}

seq_t a(idx_t n)
{
    /* n is 1‑based; n >= 1 */
    init_pow4();

    seq_t n_s = (seq_t)n;
    seq_t cum = 0;
    int L = 2;
    for (;;) {
        seq_t cnt = count_len(L);
        if (cum + cnt >= n_s) break;
        cum += cnt;
        ++L;
    }

    seq_t k = n_s - cum - 1;   /* index within length L, 0‑based */

    seq_t value = 0;
    int remaining4 = 2;
    int rem_digits = L;

    for (int pos = 0; pos < L; ++pos) {
        int start = (pos == 0) ? 1 : 0;
        for (int d = start; d <= 4; ++d) {
            int need_after = remaining4 - (d == 4 ? 1 : 0);
            int positions_left = rem_digits - 1;
            seq_t ways = count_ways(positions_left, need_after);
            if (ways > k) {
                value = value * 5 + (seq_t)d;
                remaining4 = need_after;
                --rem_digits;
                break;
            } else {
                k -= ways;
            }
        }
    }
    return value;
}
