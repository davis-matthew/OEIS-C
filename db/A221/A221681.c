/*
 * A221681  Number of 0..6 arrays of length n with each element differing from
 *          at least one neighbor by 1 or less, starting with 0.
 * https://oeis.org/A221681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of arrays of length n over {0..6} such that each
 *            element differs from at least one neighbor by at most 1,
 *            starting with 0. Recurrence: Let dp[v][f] be the number of valid
 *            partial arrays of current length ending in value v (0..6) with
 *            flag f where f = 0 -> last element already satisf...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    // number of states: value 0..6 times flag 0/1 => 7*2 = 14
    static seq_t cur[14], nxt[14];
    // zero initialization
    for (int i = 0; i < 14; ++i) cur[i] = 0;
    // start with array [0], flag 1 (needs right neighbor)
    cur[0 * 2 + 1] = 1;

    // iterate n-1 times to add remaining positions
    for (idx_t step = 1; step < n; ++step) {
        // clear next
        for (int i = 0; i < 14; ++i) nxt[i] = 0;
        for (int v = 0; v <= 6; ++v) {
            for (int f = 0; f <= 1; ++f) {
                seq_t ways = cur[v * 2 + f];
                if (ways == 0) continue;
                for (int x = 0; x <= 6; ++x) {
                    int diff = v > x ? v - x : x - v;
                    bool ok = (f == 0) || (diff <= 1);
                    if (!ok) continue;
                    int new_f = (diff <= 1) ? 0 : 1;
                    nxt[x * 2 + new_f] += ways;
                }
            }
        }
        // swap
        for (int i = 0; i < 14; ++i) cur[i] = nxt[i];
    }

    // sum over states where last flag is 0 (satisfied by left)
    seq_t total = 0;
    for (int v = 0; v <= 6; ++v) {
        total += cur[v * 2 + 0];
    }
    return total;
}
