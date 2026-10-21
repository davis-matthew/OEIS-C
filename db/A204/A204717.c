/*
 * A204717  Number of (n+1) X 3 0..1 arrays with the permanents of 2 X 2
 *          subblocks nondecreasing rightwards and downwards.
 * https://oeis.org/A204717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 28, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A204717.
   The sequence counts (n+1)×3 binary arrays whose 2×2 permanents
   are nondecreasing to the right and downwards.
   The algorithm scans rows from top to bottom, treating each
   consecutive pair of rows as a state.
   For any pair of rows (p,q) we precompute the two permanents
   perm0(p,q) and perm1(p,q).  A transition from state (p,q)
   to a new row r is allowed iff:
        perm0(p,q) ≤ perm0(q,r)   &&   perm1(p,q) ≤ perm1(q,r)
        && perm0(q,r) ≤ perm1(q,r)
   The number of (n+1)×3 arrays equals the total number of
   length‑n+1 sequences of rows respecting these transitions. */

#include <stdint.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/* Precomputation of permanents and allowed transitions                  */
static bool          init_done = false;
static uint8_t       perm0[64];
static uint8_t       perm1[64];
static uint8_t       trans_cnt[64];
static uint8_t       trans_row[64][8];

static void
precompute(void)
{
    for (uint8_t a = 0; a < 8; ++a) {
        for (uint8_t b = 0; b < 8; ++b) {
            uint8_t idx = a * 8 + b;
            uint8_t a0 = (a >> 0) & 1;
            uint8_t a1 = (a >> 1) & 1;
            uint8_t a2 = (a >> 2) & 1;
            uint8_t b0 = (b >> 0) & 1;
            uint8_t b1 = (b >> 1) & 1;
            uint8_t b2 = (b >> 2) & 1;
            perm0[idx] = a0 * b1 + a1 * b0;   /* permanent of columns 0‑1 */
            perm1[idx] = a1 * b2 + a2 * b1;   /* permanent of columns 1‑2 */
        }
    }

    for (uint8_t s = 0; s < 64; ++s) {
        uint8_t p = s / 8;
        uint8_t q = s % 8;
        uint8_t cnt = 0;
        for (uint8_t r = 0; r < 8; ++r) {
            uint8_t idx_qr = q * 8 + r;
            if (perm0[s] <= perm0[idx_qr] &&
                perm1[s] <= perm1[idx_qr] &&
                perm0[idx_qr] <= perm1[idx_qr]) {
                trans_row[s][cnt++] = r;
            }
        }
        trans_cnt[s] = cnt;
    }
    init_done = true;
}

/* --------------------------------------------------------------------- */
/* Sequence function                                                    */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (!init_done) precompute();

    /* dp[s] holds the number of partial arrays ending with the state
       represented by index s = (previous_row * 8 + current_row).       */
    seq_t dp[64];
    for (int i = 0; i < 64; ++i) dp[i] = 0;

    /* Initialise with first two rows.  The only requirement for the
       very first pair is that its two permanents satisfy perm0 ≤ perm1. */
    for (uint8_t r0 = 0; r0 < 8; ++r0) {
        for (uint8_t r1 = 0; r1 < 8; ++r1) {
            uint8_t idx = r0 * 8 + r1;
            if (perm0[idx] <= perm1[idx]) {
                dp[idx] += 1;
            }
        }
    }

    /* If only two rows (n == 1), sum the counts and return.   */
    if (n == 1) {
        seq_t total = 0;
        for (int i = 0; i < 64; ++i) total += dp[i];
        return total;
    }

    /* Add the remaining rows one by one.  There will be (n‑1) extensions. */
    for (idx_t step = 1; step < n; ++step) {
        seq_t ndp[64];
        for (int i = 0; i < 64; ++i) ndp[i] = 0;

        for (uint8_t s = 0; s < 64; ++s) {
            if (dp[s] == 0) continue;
            uint8_t q = s % 8;
            for (uint8_t k = 0; k < trans_cnt[s]; ++k) {
                uint8_t r = trans_row[s][k];
                uint8_t nxt = q * 8 + r;
                ndp[nxt] += dp[s];
            }
        }
        /* copy ndp back to dp for the next iteration */
        for (int i = 0; i < 64; ++i) dp[i] = ndp[i];
    }

    /* Sum all states for the last pair of rows. */
    seq_t result = 0;
    for (int i = 0; i < 64; ++i) result += dp[i];
    return result;
}
