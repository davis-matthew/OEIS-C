/*
 * A231513  Number of nX6 0..1 arrays with no element less than a strict
 *          majority of its horizontal, vertical, diagonal and antidiagonal
 *          neighbors.
 * https://oeis.org/A231513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×6 0..1 arrays with no element less than a
 *            strict majority of its horizontal, vertical, diagonal, and
 *            antidiagonal neighbours (OEIS A231513). The algorithm builds
 *            per‑row transition tables and uses dynamic programming over rows
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Transition table entry for a pair of consecutive rows. */
struct trans_entry {
    uint8_t nxt[64];
    uint8_t len;
};
static struct trans_entry trans[4096];
static bool final_ok[4096];
static bool tables_built = false;

/* Test whether the middle row 'cur' is valid given its upper and lower rows.
   prev_exists / next_exists indicate whether those neighbours exist. */
static inline bool row_valid(uint8_t prev, uint8_t cur, uint8_t next,
                             bool prev_exists, bool next_exists)
{
    for (int j = 0; j < 6; ++j) {
        int ones = 0, total = 0;

        /* left neighbour */
        if (j > 0) {
            ++total;
            if (cur & (1U << (j - 1))) ++ones;
        }
        /* right neighbour */
        if (j < 5) {
            ++total;
            if (cur & (1U << (j + 1))) ++ones;
        }
        /* vertical neighbours */
        if (prev_exists) {
            ++total;
            if (prev & (1U << j)) ++ones;
        }
        if (next_exists) {
            ++total;
            if (next & (1U << j)) ++ones;
        }
        /* diagonal neighbours */
        if (prev_exists && j > 0) {
            ++total;
            if (prev & (1U << (j - 1))) ++ones;
        }
        if (prev_exists && j < 5) {
            ++total;
            if (prev & (1U << (j + 1))) ++ones;
        }
        if (next_exists && j > 0) {
            ++total;
            if (next & (1U << (j - 1))) ++ones;
        }
        if (next_exists && j < 5) {
            ++total;
            if (next & (1U << (j + 1))) ++ones;
        }

        int val = (cur >> j) & 1;
        if (val == 0 && (ones * 2 > total))
            return false;   /* zero with a strict majority of 1s */
    }
    return true;
}

/* Build the transition tables once. */
static void build_tables(void)
{
    for (int prev = 0; prev < 64; ++prev) {
        for (int cur = 0; cur < 64; ++cur) {
            int idx = (prev << 6) | cur;
            /* final row validity (no lower neighbour) */
            final_ok[idx] = row_valid(prev, cur, 0, true, false);

            /* list of next rows that keep the current pair valid */
            uint8_t *p = trans[idx].nxt;
            for (int nxt = 0; nxt < 64; ++nxt) {
                if (row_valid(prev, cur, nxt, true, true))
                    *p++ = (uint8_t)nxt;
            }
            trans[idx].len = (uint8_t)(p - trans[idx].nxt);
        }
    }
    tables_built = true;
}

seq_t a(idx_t n)
{
    if (!tables_built) build_tables();

    /* Special case: single row. */
    if (n == 1) {
        seq_t count = 0;
        for (uint8_t r = 0; r < 64; ++r) {
            if (row_valid(0, r, 0, false, false))
                count += 1;
        }
        return count;
    }

    static seq_t dp[4096];
    static seq_t ndp[4096];
    for (int i = 0; i < 4096; ++i) dp[i] = 0;

    /* Initialise with the first two rows (upper neighbour missing). */
    for (uint8_t r1 = 0; r1 < 64; ++r1) {
        for (uint8_t r2 = 0; r2 < 64; ++r2) {
            if (row_valid(0, r1, r2, false, true))
                dp[(r1 << 6) | r2] = 1;
        }
    }

    for (idx_t row = 3; row <= n; ++row) {
        for (int i = 0; i < 4096; ++i) ndp[i] = 0;
        for (int idx = 0; idx < 4096; ++idx) {
            seq_t val = dp[idx];
            if (!val) continue;
            uint8_t cur = idx & 63;
            const struct trans_entry *t = &trans[idx];
            for (uint8_t k = 0; k < t->len; ++k) {
                uint8_t nxt = t->nxt[k];
                ndp[(cur << 6) | nxt] += val;
            }
        }
        /* swap dp and ndp */
        for (int i = 0; i < 4096; ++i) dp[i] = ndp[i];
    }

    /* Final row validation. */
    seq_t result = 0;
    for (int idx = 0; idx < 4096; ++idx) {
        seq_t val = dp[idx];
        if (!val) continue;
        if (final_ok[idx])
            result += val;
    }
    return result;
}
