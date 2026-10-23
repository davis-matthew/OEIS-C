/*
 * A164474  Number of binary strings of length n with no substrings equal to
 *          0001 0101 or 1100.
 * https://oeis.org/A164474
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(n) is the number of binary strings of length n that do not
 *            contain * any of the forbidden substrings 0001, 0101, or 1100. *
 *            This is computed by dynamic programming over a finite automaton
 *            * (Aho-Corasick style) that tracks the longest suffix of the *
 *            processed string which is also a prefix of...
 * Range:     u8 n <= 10, u16 n <= 23, u32 n <= 48, u64 n <= 98, u128 n <= 198,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Forward declaration of the routine.  All helper data are static
 * and initialized on the first call. */
seq_t a(idx_t n);

/* ------------------------------------------------------------------ */
/*  Automaton construction (called lazily) */

/* Maximum number of states (sum of lengths of patterns + 1 root). */
#define MAX_STATES 20   /* more than enough for patterns of length 4 */

/* Transition table, failure function and output flag for each state. */
static int  _next[MAX_STATES][2];
static int  _fail[MAX_STATES];
static bool _out[MAX_STATES];
static int  _states = 0;
static bool _automaton_built = false;

/* Builds the automaton for the three forbidden patterns:
 *   0001, 0101, 1100
 */
static void _build_automaton(void)
{
    /* Initialise root state */
    _states = 1;
    for (int s = 0; s < MAX_STATES; ++s) {
        _next[s][0] = _next[s][1] = -1;
        _fail[s] = 0;
        _out[s] = false;
    }

    /* Helper to add a pattern to the trie */
    const char *patterns[] = { "0001", "0101", "1100" };
    const size_t npat = sizeof(patterns) / sizeof(patterns[0]);

    for (size_t p = 0; p < npat; ++p) {
        const char *pat = patterns[p];
        int node = 0;
        for (const char *c = pat; *c != '\0'; ++c) {
            int bit = (*c == '1') ? 1 : 0;
            if (_next[node][bit] == -1) {
                _next[node][bit] = _states++;
            }
            node = _next[node][bit];
        }
        _out[node] = true; /* suffix ending at this node is forbidden */
    }

    /* Build failure links using BFS */
    int queue[MAX_STATES];
    int qhead = 0, qtail = 0;

    /* Set failure links for depth‑1 nodes to root */
    for (int b = 0; b < 2; ++b) {
        int child = _next[0][b];
        if (child != -1) {
            _fail[child] = 0;
            queue[qtail++] = child;
        } else {
            _next[0][b] = 0; /* missing transition goes to root */
        }
    }

    while (qhead < qtail) {
        int r = queue[qhead++];
        for (int b = 0; b < 2; ++b) {
            int child = _next[r][b];
            if (child != -1) {
                _fail[child] = _next[_fail[r]][b];
                _out[child] = _out[child] || _out[_fail[child]];
                queue[qtail++] = child;
            } else {
                _next[r][b] = _next[_fail[r]][b];
            }
        }
    }

    _automaton_built = true;
}

/* ------------------------------------------------------------------ */
/*  Main routine */

seq_t a(idx_t n)
{
    /* Ensure automaton is ready */
    if (!_automaton_built) {
        _build_automaton();
    }

    /* DP arrays: we keep two rows, one for current length, one for next */
    seq_t cur[MAX_STATES] = {0,};
    seq_t nxt[MAX_STATES] = {0,};

    cur[0] = 1; /* empty string */

    for (idx_t i = 0; i < n; ++i) {
        /* reset next row */
        for (int s = 0; s < _states; ++s) {
            nxt[s] = 0;
        }

        /* transition from each state */
        for (int s = 0; s < _states; ++s) {
            seq_t val = cur[s];
            if (val == 0) continue;
            for (int b = 0; b < 2; ++b) {
                int t = _next[s][b];
                if (_out[t]) continue; /* forbidden suffix reached */
                nxt[t] = nxt[t] + val;
            }
        }

        /* copy next to cur for next iteration */
        for (int s = 0; s < _states; ++s) {
            cur[s] = nxt[s];
        }
    }

    /* sum over all non‑forbidden states to get final count */
    seq_t total = 0;
    for (int s = 0; s < _states; ++s) {
        total = total + cur[s];
    }

    return total;
}
