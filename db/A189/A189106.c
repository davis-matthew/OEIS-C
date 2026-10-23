/*
 * A189106  Number of nX4 binary arrays without the pattern 1 1 0 diagonally
 *          or antidiagonally.
 * https://oeis.org/A189106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is calculated via a transfer‑matrix dynamic program over
 *            the rows of an n×4 binary array. A(1)=16, A(2)=256. For n≥3,
 *            a(n) is the number of n×4 binary matrices that avoid the
 *            forbidden triple 1,1,0 along any diagonal or antidiagonal of
 *            three consecutive rows. The recurrence is implicit in th...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Transition tables: for a given state (previous two rows)
   we pre‑compute the list of admissible next rows. */
static unsigned char allowed_r[256][16];
static unsigned char allowed_cnt[256];
static bool allowed_initialized = false;

/* Test whether three consecutive rows p (two rows back),
   q (previous), and r (current) contain a forbidden
   1,1,0 diagonal or antidiagonal pattern. */
static bool forbidden(uint8_t p, uint8_t q, uint8_t r)
{
    /* Diagonal patterns: (p[j], q[j+1], r[j+2]) for j=0,1 */
    for (int j = 0; j < 2; ++j) {
        if (((p >> j) & 1) && ((q >> (j + 1)) & 1) && ((r >> (j + 2)) & 1) == 0) {
            return true;
        }
    }
    /* Antidiagonal patterns: (p[j+2], q[j+1], r[j]) for j=0,1 */
    for (int j = 0; j < 2; ++j) {
        if (((p >> (j + 2)) & 1) && ((q >> (j + 1)) & 1) && ((r >> j) & 1) == 0) {
            return true;
        }
    }
    return false;
}

/* Initialise the transition tables once. */
static void init_allowed(void)
{
    if (allowed_initialized) return;

    for (int p = 0; p < 16; ++p) {
        for (int q = 0; q < 16; ++q) {
            int state = (p << 4) | q;
            unsigned char cnt = 0;
            for (int r = 0; r < 16; ++r) {
                if (!forbidden((uint8_t)p, (uint8_t)q, (uint8_t)r)) {
                    allowed_r[state][cnt++] = (unsigned char)r;
                }
            }
            allowed_cnt[state] = cnt;
        }
    }
    allowed_initialized = true;
}

/* Main sequence function.  It works for any order of calls
   and for any supported size of seq_t. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined but harmless */

    if (n == 1) return (seq_t)16;
    if (n == 2) return (seq_t)256;

    init_allowed();

    seq_t dp[256];
    seq_t ndp[256];

    /* Length 2: every pair of rows is admissible */
    for (int s = 0; s < 256; ++s) dp[s] = (seq_t)1;

    /* Build up to length n */
    for (idx_t len = 3; len <= n; ++len) {
        for (int s = 0; s < 256; ++s) ndp[s] = (seq_t)0;

        for (int s = 0; s < 256; ++s) {
            seq_t count = dp[s];
            if (!count) continue;
            unsigned char prev = (unsigned char)(s & 0xF);
            unsigned char cnt = allowed_cnt[s];
            for (unsigned char k = 0; k < cnt; ++k) {
                unsigned char r = allowed_r[s][k];
                int next_state = (prev << 4) | r;
                ndp[next_state] += count;
            }
        }

        /* Prepare for next iteration */
        for (int s = 0; s < 256; ++s) dp[s] = ndp[s];
    }

    /* Sum over all states to get a(n) */
    seq_t sum = (seq_t)0;
    for (int s = 0; s < 256; ++s) sum += dp[s];
    return sum;
}
