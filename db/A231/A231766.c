/*
 * A231766  Number of (2+1)X(n+1) 0..1 arrays with no element having a strict
 *          majority of its horizontal, diagonal and antidiagonal neighbors
 *          equal to one.
 * https://oeis.org/A231766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = number of 3 × (n+1) binary arrays with no element
    having a strict majority of its horizontal, diagonal and
    antidiagonal neighbors equal to one.
    Computed by a 64-state transfer‑matrix dynamic programme. */

#include <stdint.h>
#include <stdbool.h>

/* Helper: check if a column `col` satisfies the majority restriction
   with the previous column `prev` (if has_prev==true) and the next
   column `next` (if has_next==true). */
static inline bool col_ok(int col, int prev, int next,
                          bool has_prev, bool has_next)
{
    for (int r = 0; r < 3; ++r) {
        int ones = 0, nb = 0;

        /* left neighbour */
        if (has_prev) {
            ones += (prev >> r) & 1;
            nb++;
        }
        /* right neighbour */
        if (has_next) {
            ones += (next >> r) & 1;
            nb++;
        }
        /* diagonal up‑left */
        if (has_prev && r > 0) {
            ones += (prev >> (r - 1)) & 1;
            nb++;
        }
        /* diagonal down‑right */
        if (has_next && r < 2) {
            ones += (next >> (r + 1)) & 1;
            nb++;
        }
        /* antidiagonal up‑right */
        if (has_next && r > 0) {
            ones += (next >> (r - 1)) & 1;
            nb++;
        }
        /* antidiagonal down‑left */
        if (has_prev && r < 2) {
            ones += (prev >> (r + 1)) & 1;
            nb++;
        }
        if (ones > nb / 2)
            return false;
    }
    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t dp[64] = {0};
    seq_t ndp[64];

    /* initialisation (columns 0 and 1) */
    for (int col0 = 0; col0 < 8; ++col0) {
        for (int col1 = 0; col1 < 8; ++col1) {
            if (col_ok(col0, 0, col1, /*has_prev=*/false, /*has_next=*/true)) {
                int s = (col0 << 3) | col1;
                dp[s] = (seq_t)1;
            }
        }
    }

    /* add columns 2 … n (total n‑1 additions) */
    for (idx_t pos = 2; pos <= n; ++pos) {
        for (int i = 0; i < 64; ++i)
            ndp[i] = (seq_t)0;

        for (int prev = 0; prev < 8; ++prev) {
            for (int curr = 0; curr < 8; ++curr) {
                seq_t val = dp[(prev << 3) | curr];
                if (val == (seq_t)0)
                    continue;
                for (int next = 0; next < 8; ++next) {
                    if (col_ok(curr, prev, next,
                               /*has_prev=*/true, /*has_next=*/true)) {
                        int ns = (curr << 3) | next;
                        ndp[ns] += val;
                    }
                }
            }
        }
        for (int i = 0; i < 64; ++i)
            dp[i] = ndp[i];
    }

    /* final check for the last column */
    seq_t result = (seq_t)0;
    for (int prev = 0; prev < 8; ++prev) {
        for (int curr = 0; curr < 8; ++curr) {
            seq_t val = dp[(prev << 3) | curr];
            if (val == (seq_t)0)
                continue;
            if (col_ok(curr, prev, 0, /*has_prev=*/true, /*has_next=*/false))
                result += val;
        }
    }
    return result;
}
