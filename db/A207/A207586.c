/*
 * A207586  Number of nX5 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 1 and 1 1 1 vertically.
 * https://oeis.org/A207586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 20, u64 n <= 45, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* A207586: Number of n×5 binary arrays with no horizontal 000 or 001
   and no vertical 001 or 111.  Computed by dynamic programming over
   the last two rows.  The recurrence is
   a(1)=16, a(2)=256, a(n)=∑_{i,j,k} a_{i,j} * T(i,j,k)
   where T(i,j,k)=1 if rows i,j,k satisfy the vertical constraints,
   otherwise 0.  This implementation does not cache results across
   different n values, only the transition table. */

#define R 16                /* number of admissible 5‑bit rows  */
#define P (R * R)           /* number of pairs of rows          */

static bool initialized = false;
static int  rows[R];
static bool trans[R][R][R];
static seq_t cur[P];
static seq_t nxt[P];

/* Helper: test a single 5‑bit row for horizontal patterns 000 and 001 */
static bool good_row(int r)
{
    for (int i = 0; i + 2 < 5; ++i) {
        int b0 = (r >> i)     & 1;
        int b1 = (r >> (i+1)) & 1;
        int b2 = (r >> (i+2)) & 1;
        if (b0 == 0 && b1 == 0 && (b2 == 0 || b2 == 1))
            return false;
    }
    return true;
}

/* Build the set of admissible rows and the transition table once. */
static void init(void)
{
    int cnt = 0;
    for (int r = 0; r < 32; ++r)
        if (good_row(r))
            rows[cnt++] = r;

    for (int i = 0; i < R; ++i)
        for (int j = 0; j < R; ++j)
            for (int k = 0; k < R; ++k) {
                bool ok = true;
                for (int col = 0; col < 5; ++col) {
                    int a = (rows[i] >> col) & 1;
                    int b = (rows[j] >> col) & 1;
                    int c = (rows[k] >> col) & 1;
                    if (a == 0 && b == 0 && c == 1) { ok = false; break; }
                    if (a == 1 && b == 1 && c == 1) { ok = false; break; }
                }
                trans[i][j][k] = ok;
            }
    initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)16;
    if (n == 2) return (seq_t)256;

    if (!initialized) init();

    /* start with all length‑2 sequences counted as 1 */
    for (int i = 0; i < P; ++i)
        cur[i] = (seq_t)1;

    for (idx_t r = 3; r <= n; ++r) {
        for (int i = 0; i < P; ++i)
            nxt[i] = (seq_t)0;
        for (int i = 0; i < R; ++i)
            for (int j = 0; j < R; ++j) {
                seq_t val = cur[i * R + j];
                if (val == 0) continue;
                for (int k = 0; k < R; ++k)
                    if (trans[i][j][k])
                        nxt[j * R + k] += val;
            }
        for (int i = 0; i < P; ++i)
            cur[i] = nxt[i];
    }

    seq_t total = (seq_t)0;
    for (int i = 0; i < P; ++i)
        total += cur[i];
    return total;
}
