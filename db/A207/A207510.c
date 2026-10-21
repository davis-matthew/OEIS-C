/*
 * A207510  Number of nX4 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 1 and 1 1 1 vertically.
 * https://oeis.org/A207510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   computed by a transfer‑matrix DP. Rows are 4‑bit patterns
 *            avoiding the horizontal patterns 000 and 011. There are 9 such
 *            patterns. For n≥3 we keep the last two rows as a state and count
 *            all admissible next rows. Vertically, for each column the 3‑bit
 *            pattern (p, q, r) cannot be 001 or 111. Complex...
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 29, u64 n <= 67, u128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

static const uint8_t VALID_ROWS[] = {2,4,5,9,10,12,13,14,15};
#define NUM_ROWS 9                    /* number of admissible rows */

/* allowed_mask[p*NUM_ROWS+q] contains a bitmask of all r such that
   rows p,q,r form a valid vertical triple (for all four columns).          */
static uint16_t allowed_mask[NUM_ROWS*NUM_ROWS];
static bool allowed_initialized = false;

/* Pre‑compute the allowed transitions between pairs of rows. */
static void init_allowed(void)
{
    if (allowed_initialized) return;
    for (int pi = 0; pi < NUM_ROWS; ++pi) {
        uint8_t p = VALID_ROWS[pi];
        for (int qi = 0; qi < NUM_ROWS; ++qi) {
            uint8_t q = VALID_ROWS[qi];
            uint16_t mask = 0;
            for (int ri = 0; ri < NUM_ROWS; ++ri) {
                uint8_t r = VALID_ROWS[ri];
                bool ok = true;
                for (int col = 0; col < 4 && ok; ++col) {
                    uint8_t a = (p >> col) & 1;
                    uint8_t b = (q >> col) & 1;
                    uint8_t c = (r >> col) & 1;
                    if ((a == 0 && b == 0 && c == 1) ||
                        (a == 1 && b == 1 && c == 1))
                        ok = false;
                }
                if (ok) mask |= (1u << ri);
            }
            allowed_mask[pi*NUM_ROWS + qi] = mask;
        }
    }
    allowed_initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                /* out of offset, unused */
    if (n == 1) return (seq_t)9;
    if (n == 2) return (seq_t)81;

    init_allowed();

    /* DP arrays: dp[index] = count of sequences ending with last two rows
       indexed by index = p*NUM_ROWS + q.                               */
    seq_t dp[NUM_ROWS*NUM_ROWS]    = {0};
    seq_t newdp[NUM_ROWS*NUM_ROWS] = {0};

    /* Initialize for the first two rows (no vertical restriction yet). */
    for (int p = 0; p < NUM_ROWS; ++p)
        for (int q = 0; q < NUM_ROWS; ++q)
            dp[p*NUM_ROWS + q] = (seq_t)1;

    /* Build rows from the third up to n. */
    for (idx_t step = 3; step <= n; ++step) {
        /* clear newdp */
        for (int i = 0; i < NUM_ROWS*NUM_ROWS; ++i) newdp[i] = (seq_t)0;

        for (int pi = 0; pi < NUM_ROWS; ++pi) {
            for (int qi = 0; qi < NUM_ROWS; ++qi) {
                seq_t c = dp[pi*NUM_ROWS + qi];
                if (c == (seq_t)0) continue;
                uint16_t mask = allowed_mask[pi*NUM_ROWS + qi];
                /* iterate over set bits of mask */
                for (int ri = 0; ri < NUM_ROWS; ++ri)
                    if (mask & (1u << ri)) {
                        int idx_next = qi*NUM_ROWS + ri;
                        newdp[idx_next] = newdp[idx_next] + c;
                    }
            }
        }
        /* swap dp and newdp */
        for (int i = 0; i < NUM_ROWS*NUM_ROWS; ++i) dp[i] = newdp[i];
    }

    /* Sum all dp states to obtain the total number of arrays. */
    seq_t total = (seq_t)0;
    for (int i = 0; i < NUM_ROWS*NUM_ROWS; ++i)
        total = total + dp[i];

    return total;
}
