/*
 * A045254  Numbers whose base-5 representation contains exactly one 1 and
 *          three 4's.
 * https://oeis.org/A045254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the nth smallest positive integer whose base‑5 representation
 * contains exactly one digit 1 and exactly three digits 4.  The sequence is
 * indexed starting at 1.
 */
#include <stdint.h>
#include <stdbool.h>

static uint64_t C[32][32];
static uint64_t pow3[32];
static bool init = false;

static void init_combinatorics(void)
{
    if (init) return;
    for (int n = 0; n < 32; ++n) {
        C[n][0] = C[n][n] = 1;
        for (int k = 1; k < n; ++k) C[n][k] = C[n - 1][k - 1] + C[n - 1][k];
    }
    pow3[0] = 1;
    for (int i = 1; i < 32; ++i) pow3[i] = pow3[i - 1] * 3ULL;
    init = true;
}

static seq_t count_len(int L)
{
    if (L < 4) return 0;
    seq_t total = (seq_t)L * C[L - 1][3];
    total *= pow3[L - 4];
    seq_t leading = (seq_t)(L - 1) * C[L - 2][3];
    leading *= pow3[L - 4];
    return total - leading;
}

seq_t a(idx_t n)
{
    init_combinatorics();
    idx_t remaining = n;
    int L = 4;
    /* locate the length that contains the nth number */
    while (1) {
        seq_t cnt = count_len(L);
        if ((seq_t)remaining <= cnt) break;
        remaining -= (idx_t)cnt;
        ++L;
    }

    seq_t value = 0;
    int need1 = 1; /* one digit '1' still to place */
    int need4 = 3; /* three digits '4' still to place */

    for (int pos = 0; pos < L; ++pos) {
        int maxdigit = 4;
        int mindigit = 0;
        if (pos == 0) mindigit = 1; /* no leading zero */

        for (int d = mindigit; d <= maxdigit; ++d) {
            int r1 = need1;
            int r4 = need4;
            if (d == 1) {
                if (r1 == 0) continue;
                --r1;
            } else if (d == 4) {
                if (r4 == 0) continue;
                --r4;
            }
            int m = L - pos - 1; /* remaining positions */
            if (r1 < 0 || r4 < 0 || r1 + r4 > m) continue;

            seq_t ways = (seq_t)C[m][r1] * C[m - r1][r4];
            ways *= pow3[m - (r1 + r4)];
            if ((seq_t)remaining > ways) {
                remaining -= (idx_t)ways;
                continue;
            }

            value = value * 5 + d;
            need1 = r1;
            need4 = r4;
            break;
        }
    }
    return value;
}
