/*
 * A202416  Number of (n+2)X5 binary arrays with no more than one of any
 *          consecutive three bits set in any row or column.
 * https://oeis.org/A202416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 11, u64 n <= 24, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = number of (n+2)×5 binary arrays with no more than
   one of any consecutive three bits set in any row or column.
   The algorithm uses dynamic programming over the last two rows,
   treating each row as a 5‑bit pattern.  For each allowed row pattern
   we pre‑compute transitions to the next row that satisfy the
   column constraint.  The complexity is O(n) with a tiny constant. */

#include <stdbool.h>
#include <stddef.h>

/* enumerate all 5‑bit patterns that do not contain two 1's
   within any consecutive three bits; there are exactly 9 such patterns */
static bool initialized = false;
static int allowed[9];
static int allowed_count;
static short trans[9][9][9];         /* trans[i][j][k] = index of allowed[k] */
static unsigned char trans_len[9][9];

/* build auxiliary tables once */
static void init(void)
{
    if (initialized) return;

    allowed_count = 0;
    for (int mask = 0; mask < 32; ++mask) {
        bool ok = true;
        for (int i = 0; i < 3; ++i) {
            int bits = (mask >> i) & 7;           /* 3 bits: mask[i],mask[i+1],mask[i+2] */
            int sum = ((bits >> 0) & 1) + ((bits >> 1) & 1) + ((bits >> 2) & 1);
            if (sum > 1) { ok = false; break; }
        }
        if (ok) {
            allowed[allowed_count++] = mask;
        }
    }

    for (int i = 0; i < allowed_count; ++i) {
        int a = allowed[i];
        for (int j = 0; j < allowed_count; ++j) {
            int b = allowed[j];
            /* rows a and b must not share a 1 in any column,
               otherwise a,b,c would contain two 1's in a column
               for any third row c. */
            if (a & b) { trans_len[i][j] = 0; continue; }

            int forb = a | b;   /* columns already occupied by a or b */
            trans_len[i][j] = 0;
            for (int k = 0; k < allowed_count; ++k) {
                int c = allowed[k];
                if ((c & forb) == 0) {  /* ensures a&c==0 and b&c==0 */
                    trans[i][j][trans_len[i][j]++] = (short)k;
                }
            }
        }
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    init();

    /* number of rows in the array */
    idx_t h = n + 2;
    if (h < 2) return 0;   /* invalid, but never occurs for n>=1 */

    /* dp[i][j] = number of ways where the last two rows are
       allowed[i] (row h-2) and allowed[j] (row h-1) */
    seq_t dp_curr[9][9] = {0}, dp_next[9][9] = {0};

    /* initial state: first two rows */
    for (int i = 0; i < allowed_count; ++i) {
        for (int j = 0; j < allowed_count; ++j) {
            if ((allowed[i] & allowed[j]) == 0) {
                dp_curr[i][j] = 1;
            }
        }
    }

    /* number of transitions to add new rows after the first two */
    idx_t steps = h - 2;
    for (idx_t s = 0; s < steps; ++s) {
        for (int i = 0; i < allowed_count; ++i) {
            for (int j = 0; j < allowed_count; ++j) {
                dp_next[i][j] = 0;
            }
        }

        for (int i = 0; i < allowed_count; ++i) {
            for (int j = 0; j < allowed_count; ++j) {
                seq_t cur = dp_curr[i][j];
                if (!cur) continue;
                unsigned char len = trans_len[i][j];
                for (unsigned char t = 0; t < len; ++t) {
                    short k = trans[i][j][t];
                    dp_next[j][k] += cur;
                }
            }
        }

        /* copy next to current for next iteration */
        for (int i = 0; i < allowed_count; ++i) {
            for (int j = 0; j < allowed_count; ++j) {
                dp_curr[i][j] = dp_next[i][j];
            }
        }
    }

    /* sum all final states */
    seq_t total = 0;
    for (int i = 0; i < allowed_count; ++i) {
        for (int j = 0; j < allowed_count; ++j) {
            total += dp_curr[i][j];
        }
    }
    return total;
}
