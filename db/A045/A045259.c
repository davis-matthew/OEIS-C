/*
 * A045259  Numbers whose base-5 representation contains exactly three 1's and
 *          no 4's.
 * https://oeis.org/A045259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045259: Numbers whose base‑5 representation contains exactly three 1's
   and no 4's. The function enumerates them in increasing order by counting
   combinations. */
#include <stdint.h>

static idx_t pow3[64];
static int pow3_init = 0;

static void init_pow3(void) {
    if (pow3_init) return;
    pow3[0] = 1;
    for (idx_t i = 1; i < 64; ++i) pow3[i] = pow3[i - 1] * 3;
    pow3_init = 1;
}

static idx_t comb(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    idx_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i) / i;
    }
    return res;
}

static idx_t count_len(idx_t L) {
    if (L < 3) return 0;
    idx_t cnt1 = comb(L - 1, 2) * pow3[L - 3];  // leading digit = 1
    idx_t cnt2 = 0;
    if (L >= 4) {
        cnt2 = 2 * comb(L - 1, 3) * pow3[L - 4];  // leading digit = 2 or 3
    }
    return cnt1 + cnt2;
}

seq_t a(idx_t n) {
    init_pow3();

    /* Find length of the number */
    idx_t cum = 0;
    idx_t L = 3;
    while (1) {
        idx_t cnt = count_len(L);
        if (cum + cnt >= n) break;
        cum += cnt;
        ++L;
    }
    idx_t rank = n - cum; /* 1‑based index within this length */

    seq_t result = 0;
    idx_t ones_used = 0;

    for (idx_t pos = 0; pos < L; ++pos) {
        for (int d = 0; d <= 3; ++d) {
            if (pos == 0 && d == 0) continue;          /* no leading zero */
            idx_t new_ones = ones_used + (d == 1 ? 1 : 0);
            if (new_ones > 3) continue;

            idx_t rem_pos = L - pos - 1;
            idx_t ones_needed = 3 - new_ones;
            if (ones_needed > rem_pos) continue;

            idx_t ways = comb(rem_pos, ones_needed);
            idx_t nonone_pos = rem_pos - ones_needed;
            ways *= pow3[nonone_pos];

            if (rank > ways) {
                rank -= ways;
                continue;
            }

            result = result * 5 + (seq_t)d;
            ones_used = new_ones;
            break;
        }
    }
    return result;
}
