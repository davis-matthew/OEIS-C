/*
 * A207905  Number of nX5 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 0 and 0 1 1 vertically.
 * https://oeis.org/A207905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 binary arrays avoiding * horizontally the
 *            patterns 0 0 0 and 0 1 0, * vertically the patterns 0 0 0 and 0
 *            1 1. * Computed by dynamic programming with a two‑row state. *
 *            a(1) = 15, a(2) = 225, ... (OEIS A207905)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 19, u64 n <= 45, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Sequence element and index types are provided by the build. */
extern seq_t a(idx_t n);

static int initialized = 0;

/* 32 possible rows (5 bits). */
#define NUM_ROWS 32
#define NUM_STATE (NUM_ROWS * NUM_ROWS)

/* Transition storage. */
#define MAX_TRANS 32 /* maximal outgoing transitions per state */
static uint16_t transList[NUM_STATE][MAX_TRANS];
static uint8_t transCount[NUM_STATE];

/* Horizontal valid rows. */
static uint8_t rowValid[NUM_ROWS];

/* DP arrays for two consecutive layers. */
static seq_t dpCur[NUM_STATE];
static seq_t dpNext[NUM_STATE];

/* Helper to check horizontal validity of a row pattern. */
static void init_row_valid(void)
{
    for (int r = 0; r < NUM_ROWS; ++r) {
        int ok = 1;
        for (int c = 0; c <= 2; ++c) {
            int b0 = (r >> c) & 1;
            int b1 = (r >> (c + 1)) & 1;
            int b2 = (r >> (c + 2)) & 1;
            if ((b0 == 0 && b1 == 0 && b2 == 0) || (b0 == 0 && b1 == 1 && b2 == 0)) {
                ok = 0;
                break;
            }
        }
        rowValid[r] = ok;
    }
}

/* Helper to check vertical validity for a triplet of rows. */
static int vertical_ok(int prev, int cur, int nxt)
{
    for (int c = 0; c < 5; ++c) {
        int a = (prev >> c) & 1;
        int b = (cur >> c) & 1;
        int d = (nxt >> c) & 1;
        if ((a == 0 && b == 0 && d == 0) || (a == 0 && b == 1 && d == 1))
            return 0;
    }
    return 1;
}

/* Precompute transition list for all states. */
static void init_transitions(void)
{
    for (int a = 0; a < NUM_ROWS; ++a) {
        if (!rowValid[a]) continue;
        for (int b = 0; b < NUM_ROWS; ++b) {
            if (!rowValid[b]) continue;
            int id = a * NUM_ROWS + b;
            transCount[id] = 0;
            for (int nxt = 0; nxt < NUM_ROWS; ++nxt) {
                if (!rowValid[nxt]) continue;
                if (vertical_ok(a, b, nxt))
                    transList[id][transCount[id]++] = (uint16_t)nxt;
            }
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (!initialized) {
        init_row_valid();
        init_transitions();
        initialized = 1;
    }

    /* a(1) is just the number of horizontal‑valid rows. */
    if (n == 1)
        return (seq_t)15; /* 15 valid rows */

    /* For n >= 2, start with all unordered first two rows. */
    for (int id = 0; id < NUM_STATE; ++id)
        dpCur[id] = (seq_t)0;

    for (int a = 0; a < NUM_ROWS; ++a) {
        if (!rowValid[a]) continue;
        for (int b = 0; b < NUM_ROWS; ++b) {
            if (!rowValid[b]) continue;
            int id = a * NUM_ROWS + b;
            dpCur[id] = (seq_t)1;
        }
    }

    /* For n=2 we are finished. For larger n, iterate. */
    for (idx_t rows = 3; rows <= n; ++rows) {
        /* Clear next layer */
        for (int id = 0; id < NUM_STATE; ++id)
            dpNext[id] = (seq_t)0;

        for (int id = 0; id < NUM_STATE; ++id) {
            seq_t val = dpCur[id];
            if (val == (seq_t)0) continue;
            int b = id % NUM_ROWS;
            uint8_t cnt = transCount[id];
            for (uint8_t k = 0; k < cnt; ++k) {
                int nxt = transList[id][k];
                int new_id = b * NUM_ROWS + nxt;
                dpNext[new_id] += val;
            }
        }

        /* Swap layers */
        for (int id = 0; id < NUM_STATE; ++id)
            dpCur[id] = dpNext[id];
    }

    /* Sum all states: every state represents a complete n×5 matrix. */
    seq_t result = (seq_t)0;
    for (int id = 0; id < NUM_STATE; ++id)
        result += dpCur[id];

    return result;
}
