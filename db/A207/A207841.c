/*
 * A207841  Number of nX4 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 0 and 1 1 1 vertically.
 * https://oeis.org/A207841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: Number of n×4 binary arrays avoiding
   horizontally 000 and 001, and vertically 000 and 111.
   The algorithm uses dynamic programming over the ten admissible
   row patterns.  No external tables are stored in the final result. */

#include <stdbool.h>
#include <stdint.h>

/* List of the 10 admissible row patterns (4‑bit values).
   Only patterns that contain no horizontal 000 or 001 run are allowed. */
static const uint8_t allowedRows[10] = {4,5,6,7,10,11,12,13,14,15};

/* Tables constructed once: for each ordered pair of rows (a,b) we store
   the list of rows c that can follow a and b without creating a vertical
   forbidden triple.  The list sizes are at most 10. */
static uint8_t _nextList[10][10][10];
static uint8_t _nextCount[10][10];
static bool _initialized = false;

/* Build the transition tables on first use.  Used only once. */
static void init_transitions(void)
{
    if (_initialized) return;
    _initialized = true;

    for (int a = 0; a < 10; ++a) {
        for (int b = 0; b < 10; ++b) {
            _nextCount[a][b] = 0;
            for (int c = 0; c < 10; ++c) {
                bool ok = true;
                /* Check vertical restriction on all 4 columns. */
                for (int j = 0; j < 4; ++j) {
                    uint8_t bitA = (allowedRows[a] >> j) & 1u;
                    uint8_t bitB = (allowedRows[b] >> j) & 1u;
                    uint8_t bitC = (allowedRows[c] >> j) & 1u;
                    if (bitA == bitB && bitB == bitC) { ok = false; break; }
                }
                if (ok) {
                    uint8_t idx = _nextCount[a][b];
                    _nextList[a][b][idx] = (uint8_t)c;
                    _nextCount[a][b] = idx + 1;
                }
            }
        }
    }
}

/* Main sequence function (external linkage). */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)10;   /* a(1) = 10 */
    if (n == 2) return (seq_t)100;  /* a(2) = 100 */

    init_transitions();

    /* dp[a][b] = number of ways to fill rows 1..current
       with the last two rows being patterns a and b. */
    seq_t dp[10][10];
    seq_t nextdp[10][10];

    /* After two rows, any pair of admissible rows is possible. */
    for (int i = 0; i < 10; ++i)
        for (int j = 0; j < 10; ++j)
            dp[i][j] = (seq_t)1;

    /* Build rows 3..n one by one. */
    for (idx_t row = 3; row <= n; ++row) {
        /* clear nextdp */
        for (int i = 0; i < 10; ++i)
            for (int j = 0; j < 10; ++j)
                nextdp[i][j] = (seq_t)0;

        /* transition from (a,b) to (b,c). */
        for (int a = 0; a < 10; ++a) {
            for (int b = 0; b < 10; ++b) {
                seq_t ways = dp[a][b];
                if (ways == (seq_t)0) continue;
                uint8_t cnt = _nextCount[a][b];
                for (uint8_t k = 0; k < cnt; ++k) {
                    uint8_t c = _nextList[a][b][k];
                    nextdp[b][c] += ways;
                }
            }
        }

        /* prepare for next iteration */
        for (int i = 0; i < 10; ++i)
            for (int j = 0; j < 10; ++j)
                dp[i][j] = nextdp[i][j];
    }

    /* Sum over all final pairs to obtain the total number of arrays. */
    seq_t total = (seq_t)0;
    for (int a = 0; a < 10; ++a)
        for (int b = 0; b < 10; ++b)
            total += dp[a][b];

    return total;
}
