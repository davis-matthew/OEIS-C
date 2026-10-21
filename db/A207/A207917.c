/*
 * A207917  Number of nX7 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 1 0 and 1 0 1 vertically.
 * https://oeis.org/A207917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×7 binary arrays avoiding 000,101 horizontally
 *            and 010,101 vertically. Computed by a transfer‑matrix DP on rows
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define MAX_M 128
#define MAX_S (MAX_M * MAX_M)

static uint8_t row_bits[MAX_M][7];
static uint8_t patterns[MAX_M];
static uint16_t allowed_next[MAX_S][MAX_M];
static uint8_t allowed_count[MAX_S];
static seq_t currents[MAX_S];
static seq_t nexts[MAX_S];
static idx_t M = 0;            /* number of valid row patterns */
static idx_t S = 0;            /* number of states (M²) */
static bool initialized = false;

static void precompute(void) {
    /* Determine all 7‑bit rows that avoid 000 and 101 horizontally. */
    idx_t pat_index = 0;
    for (idx_t pat = 0; pat < (1U << 7); ++pat) {
        bool ok = true;
        for (idx_t j = 0; j + 2 < 7; ++j) {
            uint8_t b0 = (pat >> j)     & 1U;
            uint8_t b1 = (pat >> (j + 1)) & 1U;
            uint8_t b2 = (pat >> (j + 2)) & 1U;
            if ((b0 == 0 && b1 == 0 && b2 == 0) ||
                (b0 == 1 && b1 == 0 && b2 == 1)) {
                ok = false;
                break;
            }
        }
        if (ok) {
            patterns[pat_index] = (uint8_t)pat;
            for (idx_t j = 0; j < 7; ++j)
                row_bits[pat_index][j] = (pat >> j) & 1U;
            ++pat_index;
        }
    }
    M = pat_index;
    S = M * M;

    /* Precompute allowed transitions between pairs (a,b) -> c. */
    for (idx_t a = 0; a < M; ++a) {
        for (idx_t b = 0; b < M; ++b) {
            idx_t state = a * M + b;
            uint8_t count = 0;
            for (idx_t c = 0; c < M; ++c) {
                bool ok = true;
                for (idx_t j = 0; j < 7; ++j) {
                    uint8_t ba = row_bits[a][j];
                    uint8_t bb = row_bits[b][j];
                    uint8_t bc = row_bits[c][j];
                    if ((ba == 0 && bb == 1 && bc == 0) ||
                        (ba == 1 && bb == 0 && bc == 1)) {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    allowed_next[state][count++] = (uint16_t)c;
                }
            }
            allowed_count[state] = count;
        }
    }
}

seq_t a(idx_t n) {
    if (!initialized) {
        precompute();
        initialized = true;
    }

    /* n == 1 and n == 2 are trivial. */
    if (n == 1) {
        return (seq_t)M;
    }
    if (n == 2) {
        seq_t mm = (seq_t)M;
        return mm * mm;
    }

    /* DP over rows for n >= 3. */
    /* currents holds count for states (row_{i-2}, row_{i-1}) after processing i-1 rows. */
    for (idx_t i = 0; i < S; ++i) {
        currents[i] = (seq_t)1;          /* each pair of first two rows is admissible */
    }

    for (idx_t row = 3; row <= n; ++row) {
        /* clear next state array */
        for (idx_t i = 0; i < S; ++i) {
            nexts[i] = (seq_t)0;
        }
        for (idx_t state = 0; state < S; ++state) {
            seq_t cur = currents[state];
            if (cur == (seq_t)0) continue;
            idx_t a = state / M;
            idx_t b = state % M;
            uint8_t count = allowed_count[state];
            for (uint8_t k = 0; k < count; ++k) {
                idx_t c = allowed_next[state][k];
                idx_t next_state = b * M + c;
                nexts[next_state] += cur;
            }
        }
        /* swap currents and nexts */
        for (idx_t i = 0; i < S; ++i) {
            currents[i] = nexts[i];
        }
    }

    /* Sum all counts in final state array */
    seq_t total = (seq_t)0;
    for (idx_t i = 0; i < S; ++i) {
        total += currents[i];
    }
    return total;
}
