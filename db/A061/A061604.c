/*
 * A061604  Smallest multiple of n containing all 10 digits from 0 to 9.
 * https://oeis.org/A061604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = smallest positive multiple of n that contains all 10
 *            decimal digits (0–9). The algorithm performs a breadth‑first
 *            search on states (remainder modulo n, digit mask). Each state
 *            corresponds to a prefix of the desired number. When a state with
 *            remainder 0 and mask 1023 is reached, the path to...
 * Range:     u8 none, u16 none, u32 n <= 99, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1, but guard */

    const int MASKALL = 1023;      /* bits 0–9 set */
    const int STATES   = (int)n * 1024;
    /* allocate visited, parent, digit arrays */
    unsigned char *vis   = (unsigned char *)calloc(STATES, 1);
    int          *par    = (int          *)malloc((size_t)STATES * sizeof(int));
    unsigned char *dig    = (unsigned char *)malloc((size_t)STATES * sizeof(unsigned char));
    /* use a queue of integers of size STATES */
    int *queue = (int *)malloc((size_t)STATES * sizeof(int));
    if (!vis || !par || !dig || !queue) { /* allocation failed */
        free(vis); free(par); free(dig); free(queue);
        return 0;
    }
    memset(par, -1, (size_t)STATES * sizeof(int));

    int qh = 0, qt = 0;
    /* start state: remainder 0, mask 0 (no digits yet) */
    int start = 0;
    vis[start] = 1;
    queue[qt++] = start;

    int found = -1;

    while (qh < qt) {
        int cur = queue[qh++];
        int r = cur / 1024;
        int m = cur % 1024;

        /* Skip start state (no digits yet); require at least one digit */
        if (cur != start && r == 0 && m == MASKALL) {
            found = cur;
            break;
        }

        int firstDigit = (m == 0) ? 1 : 0;  /* no leading zero */
        for (int d = firstDigit; d <= 9; ++d) {
            int new_r   = (int)((r * 10 + d) % n);
            int new_m   = m | (1 << d);
            int nxt = new_r * 1024 + new_m;
            if (!vis[nxt]) {
                vis[nxt] = 1;
                par[nxt] = cur;
                dig[nxt] = (unsigned char)d;
                queue[qt++] = nxt;
            }
        }
    }

    /* reconstruct number if found */
    seq_t result = 0;
    if (found != -1) {
        /* collect digits in reverse order */
        unsigned char rev[256]; /* numbers won't be longer than this for tested n */
        int len = 0;
        int cur = found;
        while (par[cur] != -1) {
            rev[len++] = dig[cur];
            cur = par[cur];
        }
        /* reverse digits into result */
        for (int i = len - 1; i >= 0; --i) {
            result = result * 10 + rev[i];
        }
    }

    free(vis); free(par); free(dig); free(queue);
    return result;
}
