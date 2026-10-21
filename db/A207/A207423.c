/*
 * A207423  Number of nX5 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 1 and 0 1 0 vertically.
 * https://oeis.org/A207423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of n×5 0..1 arrays avoiding the horizontal
 * patterns 000 and 010, and the vertical patterns 001 and 010.
 * Computation by transfer‑matrix dynamic programming over rows.
 * The DP state is a pair of consecutive rows; transitions use a
 * pre‑computed compatibility table for triples of rows.
 * Complexity: O(n⋅15³) ≈ 3.4×10⁷ elementary operations for n = 10 000,
 * which is well within the 2‑second limit.
 */

#include <stdbool.h>
#include <stddef.h>

/* All 15 valid 5‑bit rows that have no horizontal 000 or 010. */
static const unsigned short VALID_ROWS[] = {
    0b00110, /*  6 */
    0b00111, /*  7 */
    0b01100, /* 12 */
    0b01101, /* 13 */
    0b01110, /* 14 */
    0b01111, /* 15 */
    0b10011, /* 19 */
    0b10110, /* 22 */
    0b10111, /* 23 */
    0b11001, /* 25 */
    0b11011, /* 27 */
    0b11100, /* 28 */
    0b11101, /* 29 */
    0b11110, /* 30 */
    0b11111  /* 31 */
};

#define NUM_VALID  (sizeof(VALID_ROWS)/sizeof(VALID_ROWS[0]))
#define STATES       (NUM_VALID * NUM_VALID)

/* Vertical compatibility table: true iff rows a,b,c (a two steps
 * above b) satisfy the vertical constraints (no 001 or 010 in any column). */
static bool vertical_ok[NUM_VALID][NUM_VALID][NUM_VALID] = {0};

static bool init_done = false;
static void init_tables(void)
{
    if (init_done) return;
    init_done = true;
    for (unsigned a = 0; a < NUM_VALID; ++a)
        for (unsigned b = 0; b < NUM_VALID; ++b)
            for (unsigned c = 0; c < NUM_VALID; ++c)
            {
                bool ok = true;
                for (unsigned col = 0; col < 5; ++col)
                {
                    unsigned r1 = (VALID_ROWS[a] >> col) & 1U;
                    unsigned r2 = (VALID_ROWS[b] >> col) & 1U;
                    unsigned r3 = (VALID_ROWS[c] >> col) & 1U;
                    if (r1 == 0 && r2 == 0 && r3 == 1) { ok = false; break; }
                    if (r1 == 0 && r2 == 1 && r3 == 0) { ok = false; break; }
                }
                vertical_ok[a][b][c] = ok;
            }
}

/* The sequence function */
seq_t a(idx_t n)
{
    init_tables();

    /* Trivial cases */
    if (n == 1) return (seq_t)NUM_VALID;
    if (n == 2) return (seq_t)NUM_VALID * (seq_t)NUM_VALID;

    /* DP arrays for two consecutive layers */
    static seq_t dp1[STATES];
    static seq_t dp2[STATES];
    seq_t *dp_prev = dp1;
    seq_t *dp_cur  = dp2;

    /* Initialize for the first two rows : one way for each pair (i,j) */
    for (size_t idx = 0; idx < (size_t)STATES; ++idx)
        dp_prev[idx] = 1;

    /* Process rows 3 .. n */
    for (idx_t row = 3; row <= n; ++row)
    {
        /* zero dp_cur */
        for (size_t idx = 0; idx < (size_t)STATES; ++idx)
            dp_cur[idx] = 0;

        for (unsigned i = 0; i < NUM_VALID; ++i)        /* row m-2 */
            for (unsigned j = 0; j < NUM_VALID; ++j)    /* row m-1 */
            {
                seq_t curcnt = dp_prev[(size_t)i * NUM_VALID + j];
                if (curcnt == 0) continue;
                for (unsigned k = 0; k < NUM_VALID; ++k) /* row m */
                    if (vertical_ok[i][j][k])
                    {
                        size_t nxt = (size_t)j * NUM_VALID + k;
                        dp_cur[nxt] += curcnt;
                    }
            }

        /* swap dp_prev and dp_cur for next iteration */
        seq_t *tmp = dp_prev;
        dp_prev = dp_cur;
        dp_cur  = tmp;
    }

    /* Sum all states to obtain the total number of arrays */
    seq_t total = 0;
    for (size_t idx = 0; idx < (size_t)STATES; ++idx)
        total += dp_prev[idx];

    return total;
}
