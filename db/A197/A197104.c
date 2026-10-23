/*
 * A197104  Half the number of nX3 0..3 arrays with no element equal to the
 *          average of its horizontal and vertical neighbors.
 * https://oeis.org/A197104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = half the number of n×3 0..3 arrays with no element equal
 *            to the average of its horizontal and vertical neighbors. The
 *            integer sequence is OEIS A197104
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint8_t row_vals[64][3];

/* Helper tables */
static bool good_top[64][64];              /* top row valid with only down neighbor */
static bool good_bottom[64][64];           /* bottom row valid with only up neighbor */
static unsigned char next_cnt[64][64];
static uint8_t next_rows[64][64][64];

/* DP tables */
static seq_t dp_cur[64][64];
static seq_t dp_next[64][64];

/* Memoization */
#define MAX_N_COMPUTE 12000
static seq_t memo[MAX_N_COMPUTE + 1];
static idx_t max_n_computed = 0;

/* Length of sequence represented in dp_cur (only valid >=2) */
static idx_t current_len = 0;

/* Decode a 6‑bit code into three 2‑bit cell values. */
static inline void decode_row(uint8_t code, uint8_t out[3])
{
    out[0] = (code >> 0) & 3;
    out[1] = (code >> 2) & 3;
    out[2] = (code >> 4) & 3;
}

/* Check whether a single cell is valid, given all existing neighbours. */
static inline bool cell_valid(int cur,
                              int left, int right,
                              int up, int down,
                              bool up_exists, bool down_exists)
{
    int sum = 0, cnt = 0;
    if (left >= 0) { sum += left; cnt++; }
    if (right >= 0) { sum += right; cnt++; }
    if (up_exists)  { sum += up;   cnt++; }
    if (down_exists){ sum += down; cnt++; }
    return !(cnt > 0 && (sum % cnt == 0) && (cur == sum / cnt));
}

/* Row validity with only horizontal neighbours (used for n = 1). */
static inline bool row_valid_single(uint8_t row)
{
    uint8_t v[3];
    decode_row(row, v);
    for (int j = 0; j < 3; ++j) {
        int cur  = v[j];
        int left  = (j > 0) ? v[j - 1] : -1;
        int right = (j < 2) ? v[j + 1] : -1;
        if (!cell_valid(cur, left, right, -1, -1, false, false))
            return false;
    }
    return true;
}

/* Row validity where the upper neighbour is missing and the lower neighbour exists. */
static inline bool row_valid_top(uint8_t top, uint8_t bottom)
{
    uint8_t topv[3], bottomv[3];
    decode_row(top, topv);
    decode_row(bottom, bottomv);
    for (int j = 0; j < 3; ++j) {
        int cur  = topv[j];
        int left  = (j > 0) ? topv[j - 1] : -1;
        int right = (j < 2) ? topv[j + 1] : -1;
        int down  = bottomv[j];
        if (!cell_valid(cur, left, right, -1, down, false, true))
            return false;
    }
    return true;
}

/* Row validity where the lower neighbour is missing and the upper neighbour exists. */
static inline bool row_valid_bottom(uint8_t upper, uint8_t bottom)
{
    uint8_t upperv[3], bottomv[3];
    decode_row(upper, upperv);
    decode_row(bottom, bottomv);
    for (int j = 0; j < 3; ++j) {
        int cur  = bottomv[j];
        int left  = (j > 0) ? bottomv[j - 1] : -1;
        int right = (j < 2) ? bottomv[j + 1] : -1;
        int up    = upperv[j];
        if (!cell_valid(cur, left, right, up, -1, true, false))
            return false;
    }
    return true;
}

/* Row validity where the middle row has both upper and lower neighbours. */
static inline bool row_valid_full(uint8_t upper, uint8_t middle, uint8_t lower)
{
    uint8_t upp[3], mid[3], low[3];
    decode_row(upper, upp);
    decode_row(middle, mid);
    decode_row(lower, low);
    for (int j = 0; j < 3; ++j) {
        int cur  = mid[j];
        int left  = (j > 0) ? mid[j - 1] : -1;
        int right = (j < 2) ? mid[j + 1] : -1;
        int up    = upp[j];
        int down  = low[j];
        if (!cell_valid(cur, left, right, up, down, true, true))
            return false;
    }
    return true;
}

/* Pre–compute all helper tables (called once). */
static void precompute(void)
{
    /* Decode all rows. */
    for (uint8_t r = 0; r < 64; ++r)
        decode_row(r, row_vals[r]);

    /* Build validity tables. */
    for (uint8_t up = 0; up < 64; ++up) {
        for (uint8_t cur = 0; cur < 64; ++cur) {
            good_top[up][cur] = row_valid_top(up, cur);
            good_bottom[up][cur] = row_valid_bottom(up, cur);

            /* Transition list for middle row validity. */
            unsigned char ncnt = 0;
            for (uint8_t low = 0; low < 64; ++low) {
                if (row_valid_full(up, cur, low)) {
                    next_rows[up][cur][ncnt++] = low;
                }
            }
            next_cnt[up][cur] = ncnt;
        }
    }
}

/* Compute a(n) for all n up to `need`.  Results are stored in memo. */
static void compute_upto(idx_t need)
{
    if (need <= max_n_computed)
        return;

    static bool tables_initialized = false;
    if (!tables_initialized) {
        precompute();
        tables_initialized = true;
    }

    /* Ensure a(1) is present. */
    if (max_n_computed < 1) {
        seq_t cnt1 = 0;
        for (uint8_t r = 0; r < 64; ++r) {
            if (row_valid_single(r))
                cnt1 += 1;
        }
        memo[0] = cnt1 / 2;
        max_n_computed = 1;
    }

    /* Ensure DP for two rows is ready. */
    if (max_n_computed < 2 || current_len < 2) {
        /* Initialize dp_cur for rows 1 and 2. */
        for (uint8_t prev = 0; prev < 64; ++prev)
            for (uint8_t curr = 0; curr < 64; ++curr)
                dp_cur[prev][curr] = good_top[prev][curr] ? 1 : 0;
        current_len = 2;

        /* Compute a(2). */
        seq_t tot = 0;
        for (uint8_t prev = 0; prev < 64; ++prev)
            for (uint8_t curr = 0; curr < 64; ++curr)
                if (good_bottom[prev][curr])
                    tot += dp_cur[prev][curr];
        memo[1] = tot / 2;
        max_n_computed = 2;
    }

    /* Extend to larger n. */
    for (idx_t target = max_n_computed + 1; target <= need; ++target) {
        /* Transition from current_len to current_len + 1. */
        for (uint8_t i = 0; i < 64; ++i)
            for (uint8_t j = 0; j < 64; ++j)
                dp_next[i][j] = 0;

        for (uint8_t up = 0; up < 64; ++up) {
            for (uint8_t cur = 0; cur < 64; ++cur) {
                seq_t ways = dp_cur[up][cur];
                if (!ways) continue;
                unsigned char ncnt = next_cnt[up][cur];
                uint8_t *list = next_rows[up][cur];
                for (unsigned char k = 0; k < ncnt; ++k)
                    dp_next[cur][list[k]] += ways;
            }
        }

        /* Swap buffers. */
        for (uint8_t i = 0; i < 64; ++i)
            for (uint8_t j = 0; j < 64; ++j)
                dp_cur[i][j] = dp_next[i][j];
        current_len++;

        /* Compute a(current_len). */
        seq_t tot = 0;
        for (uint8_t up = 0; up < 64; ++up)
            for (uint8_t cur = 0; cur < 64; ++cur)
                if (good_bottom[up][cur])
                    tot += dp_cur[up][cur];
        memo[target - 1] = tot / 2;
        max_n_computed = target;
    }
}

/*------------------------------------------------------------*/

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* undefined for n = 0, but callers guarantee n >= 1 */
    compute_upto(n);
    return memo[n - 1];
}
