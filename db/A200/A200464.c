/*
 * A200464  Number of 0..3 arrays x(0..n-1) of n elements with each no smaller
 *          than the sum of its four previous neighbors modulo 4.
 * https://oeis.org/A200464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 0..3 arrays of length n such that for every
 *            position i, the element x[i] is at least the sum of its at most
 *            four preceding neighbors modulo 4
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Precompute for each of the 256 possible 4‑tuples (a,b,c,d) the
       threshold AND the transition to the next state when a new element
       x is appended. */
    static unsigned char thr[256];
    static unsigned char next_state[256][4];
    static bool init = false;
    if (!init) {
        for (int s = 0; s < 256; ++s) {
            unsigned char a = (s >> 6) & 3;
            unsigned char b = (s >> 4) & 3;
            unsigned char c = (s >> 2) & 3;
            unsigned char d = s & 3;
            unsigned char sum = a + b + c + d;
            thr[s] = sum & 3;          /* sum modulo 4 */
            for (int x = 0; x < 4; ++x) {
                next_state[s][x] = (b << 6) | (c << 4) | (d << 2) | x;
            }
        }
        init = true;
    }

    /* DP arrays: cur[s] = number of length‑step sequences ending with state s. */
    seq_t cur[256];
    seq_t next[256];
    for (int i = 0; i < 256; ++i) cur[i] = 0;
    cur[0] = 1;                       /* start from the empty state (0,0,0,0) */

    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < 256; ++i) next[i] = 0;
        for (int s = 0; s < 256; ++s) {
            seq_t cnt = cur[s];
            if (!cnt) continue;
            unsigned char min_needed = thr[s];
            for (int x = min_needed; x < 4; ++x) {
                next[next_state[s][x]] += cnt;
            }
        }
        for (int i = 0; i < 256; ++i) cur[i] = next[i];
    }

    seq_t total = 0;
    for (int i = 0; i < 256; ++i) total += cur[i];
    return total;
}
