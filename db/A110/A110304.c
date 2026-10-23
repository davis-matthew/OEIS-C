/*
 * A110304  Least alternating multiple of alternators.
 * https://oeis.org/A110304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = least alternating multiple of n, 0 if none exists
 * Range:     u8 n <= 21, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return 0;          /* not defined for 0, but guard */
    if (n % (idx_t)20 == (idx_t)0) return 0; /* known non‑alternators */

    /* State: (remainder, last_parity) -> index = remainder*2 + parity   */
    idx_t N = n;
    idx_t state_cnt = N * 2;               /* maximum number of states   */
    idx_t sentinel = state_cnt;            /* special value for start    */

    /* Allocate state arrays */
    bool  *vis   = (bool  *)malloc(state_cnt * sizeof(bool));
    idx_t *par   = (idx_t *)malloc(state_cnt * sizeof(idx_t));
    uint8_t *dig = (uint8_t *)malloc(state_cnt * sizeof(uint8_t));
    if (!vis || !par || !dig) { free(vis); free(par); free(dig); return 0; }

    for (idx_t i = 0; i < state_cnt; ++i)vis[i] = false, par[i] = sentinel, dig[i] = 0;

    /* Queue for BFS */
    idx_t *q    = (idx_t *)malloc(state_cnt * sizeof(idx_t));
    idx_t   front = 0, back = 0;
    if (!q) { free(vis); free(par); free(dig); return 0; }

    /* Push all one‑digit states (1‑9) */
    for (int d = 1; d <= 9; ++d) {
        idx_t rem   = (idx_t)(d % N);
        idx_t state = rem * 2 + (d & 1);
        if (vis[state]) continue;
        vis[state] = true;
        par[state] = sentinel;              /* start state */
        dig[state] = (uint8_t)d;
        q[back++] = state;
    }

    idx_t found_state = sentinel;
    while (front < back) {
        idx_t cur   = q[front++];
        idx_t rem   = cur / 2;
        idx_t lastp = cur & 1;
        if (rem == (idx_t)0) {              /* solution found */
            found_state = cur;
            break;
        }
        for (int d = 0; d <= 9; ++d) {
            if (((d & 1) == lastp)) continue;  /* parity must alternate */
            idx_t rem2   = (idx_t)((rem * (idx_t)10 + (idx_t)d) % N);
            idx_t state2 = rem2 * 2 + (d & 1);
            if (vis[state2]) continue;
            vis[state2] = true;
            par[state2] = cur;
            dig[state2] = (uint8_t)d;
            q[back++]   = state2;
        }
    }

    seq_t result = 0;
    if (found_state != sentinel) {
        /* Reconstruct digits in reverse order */
        int    rev[20001];           /* enough for n <= 10000 */
        int    rev_len = 0;
        idx_t  cur = found_state;
        while (true) {
            rev[rev_len++] = (int)dig[cur];
            if (par[cur] == sentinel) break;
            cur = par[cur];
        }
        /* Build the number from most significant digit to least */
        for (int i = rev_len - 1; i >= 0; --i) {
            result = result * (seq_t)10 + (seq_t)rev[i];
        }
    }

    free(vis);
    free(par);
    free(dig);
    free(q);
    return result;
}
