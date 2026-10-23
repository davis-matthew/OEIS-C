/*
 * A134562  Array T by antidiagonals: T(n,k) = k-th number whose formal base-3
 *          representation has exactly n terms. ("Formal" means that all the
 *          nonzero coefficients are 1's.).
 * https://oeis.org/A134562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where i+j is the antidiagonal index, T(i,j) = j-th
 *            smallest integer whose base‑3 digit sum equals i
 * Range:     u8 n <= 21, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes */
#include <stdint.h>
#include <stdlib.h>

/* Compare and limits */
static const uint64_t CAP = 1ULL << 60;      /* 1e18 approx, enough for our needs */

static uint64_t **g_rows = NULL;   /* DP table: g[pos][sum] = #ways to fill pos digits (0..2) with given sum  */
static int g_max_pos = -1;         /* highest position computed */

/* Ensure DP table is filled up to a given position (pos) */
static void ensure_g(int pos_limit) {
    if (g_rows == NULL) {
        /* Allocate first row for pos = 0 */
        g_rows = (uint64_t**)malloc(sizeof(uint64_t*));
        g_rows[0] = (uint64_t*)malloc(sizeof(uint64_t));
        g_rows[0][0] = 1ULL;
        g_max_pos = 0;
    }
    while (g_max_pos < pos_limit) {
        int pos = g_max_pos + 1;
        int size = 2 * pos + 1;
        uint64_t *row = (uint64_t*)malloc(size * sizeof(uint64_t));
        uint64_t *prev = g_rows[pos - 1];
        int prevSize = 2 * (pos - 1) + 1;
        for (int s = 0; s < size; ++s) {
            uint64_t val = 0;
            if (s < prevSize) val += prev[s];
            if (s - 1 >= 0 && s - 1 < prevSize) val += prev[s - 1];
            if (s - 2 >= 0 && s - 2 < prevSize) val += prev[s - 2];
            if (val > CAP) val = CAP;
            row[s] = val;
        }
        /* Grow the array of row pointers */
        g_rows = (uint64_t**)realloc(g_rows, (pos + 1) * sizeof(uint64_t*));
        g_rows[pos] = row;
        g_max_pos = pos;
    }
}

/* Count of numbers with digit sum = t and length L (digits) */
static uint64_t count_len(int L, int t) {
    uint64_t cnt = 0;
    if (t >= 1 && t - 1 <= 2 * (L - 1)) {
        cnt += g_rows[L - 1][t - 1];
    }
    if (t >= 2 && t - 2 <= 2 * (L - 1)) {
        uint64_t add = g_rows[L - 1][t - 2];
        cnt += add;
    }
    if (cnt > CAP) cnt = CAP;
    return cnt;
}

/* Return the k-th smallest integer whose base‑3 digit sum equals t */
static seq_t kth_number_with_digit_sum(uint64_t t, uint64_t k) {
    if (t == 0) return 0;
    /* Prepare DP tables up to necessary positions */
    ensure_g((int)t);   /* we only need rows up to t for our small k */

    /* Find minimal length L such that cumulative count >= k */
    uint64_t cum = 0;      /* use 128‑bit for safety */
    int L = (t + 1) / 2;   /* minimal possible length */
    uint64_t c = 0;
    while (1) {
        c = count_len(L, (int)t);
        cum += c;
        if (cum >= k) break;
        ++L;
    }
    uint64_t prev_cum = cum - c;
    uint64_t kprime = k - prev_cum; /* 1‑based index within this length */

    /* Reconstruct digits from most significant to least */
    seq_t value = 0;
    uint64_t rem = t;
    for (int pos = L - 1; pos >= 0; --pos) {
        int start = (pos == L - 1) ? 1 : 0;
        for (int d = start; d <= 2; ++d) {
            if (rem < (uint64_t)d) continue;
            uint64_t cnt_rest = 0;
            if (pos == 0) {
                cnt_rest = (rem - d == 0) ? 1 : 0;
            } else {
                uint64_t sum_needed = rem - d;
                if (sum_needed <= 2 * pos) {
                    cnt_rest = g_rows[pos][sum_needed];
                } else {
                    cnt_rest = 0;
                }
            }
            if (cnt_rest >= kprime) {
                value = value * 3 + (seq_t)d;
                rem -= d;
                break;
            } else {
                kprime -= cnt_rest;
            }
        }
    }
    return value;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* idx_t is 1‑based; guard against 0 */

    /* Find the antidiagonal index s such that (s-1)*s/2 >= n */
    uint64_t m = 1;
    while ((unsigned __int128)m * (m + 1) / 2 < (unsigned __int128)n) ++m;
    uint64_t prevC = (unsigned __int128)(m - 1) * m / 2;
    uint64_t i = n - prevC;            /* row index (number of terms) */
    uint64_t s = m + 1;                /* sum of coordinates = s */
    uint64_t j = s - i;                /* column index */

    /* i is the term count t, j is the k‑th within that term count */
    return kth_number_with_digit_sum(i, j);
}
