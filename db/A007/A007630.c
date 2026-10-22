/*
 * A007630  Duplicate of A002562.
 * https://oeis.org/A007630
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = (Q(n) + S2(n) + 2*S4(n)) / 8 by Burnside, where Q(n) =
 *            A000170(n) is the number of solutions to the n-queens problem,
 *            S2 = solutions symmetric under 180-degree rotation, S4 =
 *            solutions symmetric under 90-degree rotation; all counted by
 *            bitmask backtracking search
 * Range:     i8 n <= 10, i16 n <= 13, i32 n <= 16, i64 n <= 16, i128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>

/* count completions from row `row` given occupied masks; iterative bitmask backtracking */
static uint64_t q_from(int n, uint64_t all, int row, uint64_t cols, uint64_t ld, uint64_t rd) {
    uint64_t st_av[64], st_c[64], st_l[64], st_r[64];
    uint64_t cnt = 0;
    int d = row;
    if (row >= n) return 1;
    uint64_t avail = all & ~(cols | ld | rd);
    for (;;) {
        if (avail == 0) {
            if (d == row) break;
            d--;
            cols = st_c[d]; ld = st_l[d]; rd = st_r[d]; avail = st_av[d];
            continue;
        }
        uint64_t bit = avail & (~avail + 1);
        avail ^= bit;
        if (d == n - 1) { cnt++; continue; }
        st_c[d] = cols; st_l[d] = ld; st_r[d] = rd; st_av[d] = avail;
        cols |= bit;
        ld = ((ld | bit) << 1) & all;
        rd = (rd | bit) >> 1;
        d++;
        avail = all & ~(cols | ld | rd);
    }
    return cnt;
}

static uint64_t q_total(int n) {
    uint64_t all = (n == 64) ? ~(uint64_t)0 : (((uint64_t)1 << n) - 1);
    uint64_t cnt = 0;
    int half = n / 2;
    for (int c = 0; c < half; c++) {
        uint64_t bit = (uint64_t)1 << c;
        cnt += q_from(n, all, 1, bit, (bit << 1) & all, bit >> 1);
    }
    cnt *= 2;
    if (n & 1) {
        uint64_t bit = (uint64_t)1 << half;
        cnt += q_from(n, all, 1, bit, (bit << 1) & all, bit >> 1);
    }
    return cnt;
}

static int s_n;
static char s_row[64], s_col[64], s_d1[128], s_d2[128];

static int s_place(int r, int c) {
    if (s_row[r] || s_col[c] || s_d1[r + c] || s_d2[r - c + s_n - 1]) return 0;
    s_row[r] = s_col[c] = s_d1[r + c] = s_d2[r - c + s_n - 1] = 1;
    return 1;
}

static void s_unplace(int r, int c) {
    s_row[r] = s_col[c] = s_d1[r + c] = s_d2[r - c + s_n - 1] = 0;
}

static uint64_t s_rec(int k) {
    int n = s_n, r = 0;
    while (r < n && s_row[r]) r++;
    if (r == n) return 1;
    uint64_t cnt = 0;
    for (int c = 0; c < n; c++) {
        int pr[4], pc[4];
        pr[0] = r; pc[0] = c;
        if (k == 2) {
            pr[1] = n - 1 - r; pc[1] = n - 1 - c;
        } else {
            pr[1] = c;         pc[1] = n - 1 - r;
            pr[2] = n - 1 - r; pc[2] = n - 1 - c;
            pr[3] = n - 1 - c; pc[3] = r;
        }
        int placed = 0;
        while (placed < k && s_place(pr[placed], pc[placed])) placed++;
        if (placed == k) cnt += s_rec(k);
        while (placed > 0) { placed--; s_unplace(pr[placed], pc[placed]); }
    }
    return cnt;
}

static uint64_t sym_count(int n, int k) {
    s_n = n;
    memset(s_row, 0, sizeof s_row);
    memset(s_col, 0, sizeof s_col);
    memset(s_d1, 0, sizeof s_d1);
    memset(s_d2, 0, sizeof s_d2);
    if (n & 1) s_place(n / 2, n / 2);
    return s_rec(k);
}

static uint64_t memo[64];
static char memo_ok[64];

seq_t a(idx_t n) {
    if (n < 2) return (seq_t)1;
    int m = (int)n;
    if (m < 64 && memo_ok[m]) return (seq_t)memo[m];
    uint64_t q = q_total(m);
    uint64_t s180 = sym_count(m, 2);
    uint64_t s90 = (m % 4 == 2 || m % 4 == 3) ? 0 : sym_count(m, 4);
    uint64_t u = (q + s180 + 2 * s90) / 8;
    if (m < 64) { memo[m] = u; memo_ok[m] = 1; }
    return (seq_t)u;
}
