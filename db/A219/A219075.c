/*
 * A219075  Number of n X 5 binary arrays with every 0 a horizontal, diagonal
 *          or antidiagonal neighbor to some 1.
 * https://oeis.org/A219075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Number of n × 5 binary arrays such that every 0 has
 * at least one neighboring 1 among horizontal, diagonal or antidiagonal
 * positions.  This implementation uses dynamic programming over rows.
 * Complexity: O(n * 32³) which is < 2e8 elementary operations for n = 1 000 000,
 * easily below the time limit for the given constraints.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* pre‑computed tables – built once */
    static bool ready = false;
    static unsigned char satMask[32];      /* bits j where cur has a 1 at j-1 or j+1 */
    static unsigned char needTable[32][32]; /* needTable[prev][cur] = need mask for cur */

    if (!ready) {
        ready = true;
        for (int cur = 0; cur < 32; ++cur) {
            unsigned char sm = 0;
            for (int col = 0; col < 5; ++col) {
                if ((col > 0 && ((cur >> (col - 1)) & 1)) ||
                    (col < 4 && ((cur >> (col + 1)) & 1)))
                    sm |= (1 << col);
            }
            satMask[cur] = sm;
        }

        for (int prev = 0; prev < 32; ++prev) {
            for (int cur = 0; cur < 32; ++cur) {
                unsigned char need = 0;
                for (int col = 0; col < 5; ++col) {
                    if ((cur >> col) & 1) continue;   /* cell is 1 */

                    bool sat = false;
                    if (col > 0 && ((cur >> (col - 1)) & 1)) sat = true;
                    else if (col < 4 && ((cur >> (col + 1)) & 1)) sat = true;
                    else if (col > 0 && ((prev >> (col - 1)) & 1)) sat = true;
                    else if (col < 4 && ((prev >> (col + 1)) & 1)) sat = true;

                    if (!sat) need |= (1 << col);
                }
                needTable[prev][cur] = need;
            }
        }
    }

    /* DP tables: dp[mask][need] */
    seq_t dp[32][32];
    seq_t ndp[32][32];
    /* clear dp */
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            dp[i][j] = 0;

    /* initial row – no previous row, use prevMask = 0 */
    for (int cur = 0; cur < 32; ++cur) {
        unsigned char need = needTable[0][cur];
        dp[cur][need] = 1;
    }

    /* process subsequent rows */
    for (idx_t row = 1; row < n; ++row) {
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                ndp[i][j] = 0;

        for (int prev = 0; prev < 32; ++prev) {
            for (int prevNeed = 0; prevNeed < 32; ++prevNeed) {
                seq_t cnt = dp[prev][prevNeed];
                if (!cnt) continue;

                for (int cur = 0; cur < 32; ++cur) {
                    /* check that all required 0's in previous row are satisfied */
                    if ((prevNeed & ~satMask[cur]) != 0) continue;

                    unsigned char curNeed = needTable[prev][cur];
                    ndp[cur][curNeed] += cnt;
                }
            }
        }
        /* swap dp and ndp */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* sum all states with no pending requirements */
    seq_t result = 0;
    for (int mask = 0; mask < 32; ++mask)
        result += dp[mask][0];

    return result;
}
