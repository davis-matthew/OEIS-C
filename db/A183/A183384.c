/*
 * A183384  Half the number of nX5 binary arrays with no element equal to a
 *          strict majority of its king-move neighbors.
 * https://oeis.org/A183384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/2 * (# of n×5 binary arrays with no element equal to a
 *            strict majority of its king‑move neighbors)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>


static bool check_row(bool above, unsigned int a, unsigned int b,
                      bool below, unsigned int c)
{
    int cell;
    for (unsigned int j = 0; j < 5; ++j) {
        cell = (b >> j) & 1;
        int equal = 0, total = 0;

        /* Left neighbour */
        if (j > 0) {
            ++total;
            if (((b >> (j - 1)) & 1) == cell) ++equal;
        }

        /* Right neighbour */
        if (j < 4) {
            ++total;
            if (((b >> (j + 1)) & 1) == cell) ++equal;
        }

        /* Upper neighbours */
        if (above) {
            if (j > 0) {
                ++total;
                if (((a >> (j - 1)) & 1) == cell) ++equal;
            }
            ++total;
            if (((a >> j) & 1) == cell) ++equal;
            if (j < 4) {
                ++total;
                if (((a >> (j + 1)) & 1) == cell) ++equal;
            }
        }

        /* Lower neighbours */
        if (below) {
            if (j > 0) {
                ++total;
                if (((c >> (j - 1)) & 1) == cell) ++equal;
            }
            ++total;
            if (((c >> j) & 1) == cell) ++equal;
            if (j < 4) {
                ++total;
                if (((c >> (j + 1)) & 1) == cell) ++equal;
            }
        }

        if (equal * 2 > total) return false;   /* strict majority */
    }
    return true;
}

/* Constants for DP */
#define MAX_ROWS 32                    /* 2^5 possible rows */
#define MAX_STATE (MAX_ROWS * MAX_ROWS)/* previous × current states */

static unsigned char next_list[MAX_STATE][MAX_ROWS];
static unsigned char next_cnt[MAX_STATE];
static bool valid_start[MAX_ROWS][MAX_ROWS];
static bool valid_bottom[MAX_ROWS][MAX_ROWS];
static bool initialized = false;

seq_t a(idx_t n)
{
    /* Initialise transition tables on first call */
    if (!initialized) {
        for (unsigned int prev = 0; prev < MAX_ROWS; ++prev) {
            for (unsigned int curr = 0; curr < MAX_ROWS; ++curr) {
                unsigned int idx = prev * MAX_ROWS + curr;

                /* When building the first two rows, the first row has no upper neighbours */
                valid_start[prev][curr] = check_row(false, 0, prev, true, curr);

                /* The last row will have no lower neighbours */
                valid_bottom[prev][curr] = check_row(true, prev, curr, false, 0);

                /* Transition list: from (prev, curr) to (curr, nxt) */
                next_cnt[idx] = 0;
                for (unsigned int nxt = 0; nxt < MAX_ROWS; ++nxt) {
                    if (check_row(true, prev, curr, true, nxt)) {
                        next_list[idx][next_cnt[idx]++] = (unsigned char)nxt;
                    }
                }
            }
        }
        initialized = true;
    }

    /* Handle the single‑row case separately */
    if (n == 1) {
        seq_t count = 0;
        for (unsigned int r = 0; r < MAX_ROWS; ++r) {
            if (check_row(false, 0, r, false, 0))
                count += 1;
        }
        return count >> 1;   /* divide by two */
    }

    /* DP arrays */
    seq_t dp[MAX_STATE] = {0};
    seq_t next_dp[MAX_STATE] = {0};

    /* Initialise with the first two rows */
    for (unsigned int prev = 0; prev < MAX_ROWS; ++prev) {
        for (unsigned int curr = 0; curr < MAX_ROWS; ++curr) {
            if (valid_start[prev][curr])
                dp[prev * MAX_ROWS + curr] = 1;
        }
    }

    /* Process remaining rows */
    for (idx_t step = 0; step < n - 2; ++step) {
        /* Clear next DP */
        for (unsigned int i = 0; i < MAX_STATE; ++i)
            next_dp[i] = 0;

        /* Transition */
        for (unsigned int i = 0; i < MAX_STATE; ++i) {
            seq_t ways = dp[i];
            if (!ways) continue;
            unsigned int prev = i / MAX_ROWS;
            unsigned int curr = i % MAX_ROWS;
            unsigned int outcnt = next_cnt[i];
            unsigned char *outs = next_list[i];
            for (unsigned int k = 0; k < outcnt; ++k) {
                unsigned int nxt = outs[k];
                next_dp[curr * MAX_ROWS + nxt] += ways;
            }
        }
        /* Copy next_dp to dp */
        for (unsigned int i = 0; i < MAX_STATE; ++i)
            dp[i] = next_dp[i];
    }

    /* Sum over valid bottom rows */
    seq_t total = 0;
    for (unsigned int i = 0; i < MAX_STATE; ++i) {
        if (!dp[i]) continue;
        unsigned int prev = i / MAX_ROWS;
        unsigned int curr = i % MAX_ROWS;
        if (valid_bottom[prev][curr])
            total += dp[i];
    }

    return total >> 1;   /* divide by two */
}
