/*
 * A078241  Smallest multiple of n using only digits 0 and 2.
 * https://oeis.org/A078241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the smallest positive integer composed only of the digits
 *            0 and 2 that is divisible by n. The function implements a
 *            breadth‑first search over the set of remainders modulo n,
 *            keeping the shortest path of allowed digits that reaches each
 *            remainder
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 71, u64 n <= 791, u128 n <= 9998,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation note: the code works for every build where seq_t is an unsigned integer type,
   including arbitrary‑precision GNU C++ big integers. */

#include <stdlib.h>
#include <stdbool.h>

/* seq_t and idx_t are supplied by the build system */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* never used, but keeps the function total */

    /* ------------  initialise BFS data structures  ------------ */
    idx_t *queue = (idx_t *)malloc(n * sizeof(idx_t));
    idx_t head = 0, tail = 0;

    idx_t *pred = (idx_t *)malloc(n * sizeof(idx_t));           /* previous remainder in the path */
    unsigned char *dig = (unsigned char *)malloc(n * sizeof(unsigned char)); /* digit that leads to the remainder */
    bool *vis   = (bool *)calloc(n, sizeof(bool));                 /* visited flag */

    /* ------------  start from the only non‑zero digit 2  ------------ */
    idx_t start_rem = 2 % n;
    vis[start_rem] = true;
    pred[start_rem] = n;            /* sentinel: n is larger than any remainder index */
    dig[start_rem] = 2;
    queue[tail++] = start_rem;

    if (start_rem == 0) {           /* n divides 2, trivial solution */
        seq_t result = 2;
        free(queue); free(pred); free(dig); free(vis);
        return result;
    }

    /* ------------  BFS traversal  ------------ */
    idx_t target = 0;
    bool found = false;

    while (head < tail && !found) {
        idx_t cur = queue[head++];
        /* try appending 0 and 2 (0 first so that the lexicographically smaller number is found) */
        for (unsigned char d = 0; d <= 2; d += 2) {
            idx_t nxt = (cur * 10 + d) % n;
            if (!vis[nxt]) {
                vis[nxt] = true;
                pred[nxt] = cur;
                dig[nxt]  = d;
                if (nxt == 0) {          /* reached remainder 0 → solution found */
                    target = nxt;
                    found = true;
                    break;
                }
                queue[tail++] = nxt;
            }
        }
    }

    /* ------------  reconstruct the digit string  ------------ */
    unsigned char *rev = NULL;      /* digits stored in reverse order */
    size_t rev_len = 0, rev_cap = 0;

    idx_t cur = target;             /* cur == 0 */
    while (pred[cur] != n) {        /* walk back until the sentinel */
        if (rev_len == rev_cap) {
            rev_cap = rev_cap ? rev_cap * 2 : 8;
            rev = (unsigned char *)realloc(rev, rev_cap * sizeof(unsigned char));
        }
        rev[rev_len++] = dig[cur];
        cur = pred[cur];
    }
    /* add the first digit (the one leading the representation) */
    if (rev_len == rev_cap) {
        rev_cap = rev_cap ? rev_cap * 2 : 8;
        rev = (unsigned char *)realloc(rev, rev_cap * sizeof(unsigned char));
    }
    rev[rev_len++] = dig[cur];

    /* ------------  convert the digit sequence to a seq_t value  ------------ */
    seq_t value = 0;
    for (size_t i = rev_len; i > 0; --i) {
        value = value * 10 + rev[i - 1];
    }

    /* ------------  clean up and return  ------------ */
    free(queue);
    free(pred);
    free(dig);
    free(vis);
    free(rev);

    return value;
}
