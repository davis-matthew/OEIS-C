/*
 * A002187  Sprague-Grundy values for Dawson's Chess (octal game .137).
 * https://oeis.org/A002187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   Sprague-Grundy values of octal game .137, computed by mex over
 *            moves (remove 1 from a heap of 1; remove 2 leaving 0 or 1 heap;
 *            remove 3 leaving 0, 1 or 2 heaps); a(n) = a(n-34) for n >= 52
 *            (period 34, last exception at n=51)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define A002187_LIM 240

seq_t a(idx_t n) {
    static int g[A002187_LIM];
    static bool init = false;
    if (!init) {
        static const int code[4] = {0, 1, 3, 7}; /* octal digits of .137 */
        for (int m = 0; m < A002187_LIM; m++) {
            bool seen[256];
            memset(seen, 0, sizeof(seen));
            for (int k = 1; k <= 3 && k <= m; k++) {
                int d = code[k];
                int r = m - k;
                if ((d & 1) && r == 0) seen[0] = true;
                if ((d & 2) && r >= 1) seen[g[r]] = true;
                if ((d & 4) && r >= 2) {
                    for (int i = 1; i <= r - 1; i++) seen[g[i] ^ g[r - i]] = true;
                }
            }
            int mex = 0;
            while (seen[mex]) mex++;
            g[m] = mex;
        }
        init = true;
    }
    if (n >= A002187_LIM) {
        idx_t base = 100;
        n = base + (n - base) % 34;
    }
    return (seq_t)g[(long long)n];
}
