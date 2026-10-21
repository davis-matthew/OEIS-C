/*
 * A207806  Number of nX6 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 1 0 and 1 0 1 vertically.
 * https://oeis.org/A207806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A207806
   Number of n×6 binary arrays avoiding the patterns
   horizontally: 000 and 001
   vertically   : 010 and 101
   a(1) = 26, a(2) = 676, a(3) = 4374, … (see OEIS entry) */

#include <stdbool.h>

/* number of admissible 6‑bit rows (no 000 or 001 horizontally) */
#define NROWS 26
/* number of ordered pairs of rows of that kind */
#define STATES (NROWS * NROWS)

/* static data that is built once */
static unsigned char        allowed_rows[NROWS];          /* admissible rows */
static unsigned              next_mask[STATES];           /* vertical transitions */
static seq_t                 dp_cur[STATES];
static seq_t                 dp_next[STATES];
static int                   initialized = 0;

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Build the tables only once */
    if (!initialized) {
        int cnt = 0;

        /* enumerate all 6‑bit patterns and keep those that have no
           consecutive 0 0 followed by 0 or 1 horizontally. */
        for (int patt = 0; patt < 64; ++patt) {
            bool ok = true;
            for (int i = 0; i <= 3; ++i)          /* 4 triples in a 6‑bit word */
                if ( ((patt >> i) & 3) == 0 ) {   /* first two bits are 0 */
                    ok = false;
                    break;
                }
            if (ok) {
                allowed_rows[cnt] = (unsigned char)patt;
                ++cnt;
            }
        }
        /* cnt should be NROWS (26) */
        for (int i = 0; i < cnt; ++i)
            for (int j = 0; j < cnt; ++j) {
                unsigned mask = 0;
                for (int k = 0; k < cnt; ++k) {
                    bool good = true;
                    for (int col = 0; col < 6; ++col) {
                        int b0 = (allowed_rows[i] >> col) & 1;
                        int b1 = (allowed_rows[j] >> col) & 1;
                        int b2 = (allowed_rows[k] >> col) & 1;
                        /* forbid 0‑1‑0 and 1‑0‑1 vertically in any column */
                        if ((b0==0 && b1==1 && b2==0) ||
                            (b0==1 && b1==0 && b2==1)) {
                            good = false;
                            break;
                        }
                    }
                    if (good) mask |= (1U << k);
                }
                next_mask[i * cnt + j] = mask;
            }

        initialized = 1;
    }

    /* a(1) and a(2) have closed forms */
    if (n == 0) return 0;                   /* safety for n < 1 */
    if (n == 1) return (seq_t)NROWS;        /* 26 admissible rows */
    if (n == 2) return (seq_t)NROWS * (seq_t)NROWS;   /* 26² */

    /* DP: initialise for the first two rows (any ordered pair is allowed) */
    for (int i = 0; i < STATES; ++i) dp_cur[i] = 0;
    for (int i = 0; i < NROWS; ++i)
        for (int j = 0; j < NROWS; ++j)
            dp_cur[i * NROWS + j] = (seq_t)1;

    /* Process rows 3 … n */
    for (idx_t step = 3; step <= n; ++step) {
        for (int i = 0; i < STATES; ++i) dp_next[i] = 0;

        for (int s = 0; s < STATES; ++s) {
            seq_t val = dp_cur[s];
            if (val == 0) continue;

            int j = s % NROWS;                /* last row of the pair */
            unsigned mask = next_mask[s];     /* admissible k for pair (i,j) */

            /* iterate all k with mask bit set */
            for (int k = 0; k < NROWS; ++k)
                if (mask & (1U << k))
                    dp_next[j * NROWS + k] += val;
        }

        /* swap dp_cur and dp_next */
        for (int i = 0; i < STATES; ++i) dp_cur[i] = dp_next[i];
    }

    /* sum all states to obtain the total number of arrays */
    seq_t ans = 0;
    for (int i = 0; i < STATES; ++i) ans += dp_cur[i];
    return ans;
}
