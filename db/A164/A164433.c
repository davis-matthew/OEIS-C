/*
 * A164433  Number of binary strings of length n with no substrings equal to
 *          0000 0101 or 0111.
 * https://oeis.org/A164433
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 23, u32 n <= 48, u64 n <= 99, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A164433  
    Number of binary strings of length n with no substrings equal to  
    0000, 0101 or 0111.  The sequence is computed with a simple
    deterministic finite automaton (Aho‑Corasick) that recognises
    the forbidden substrings.  The DP over the automaton states runs
    in O(n) time with a very small constant.
*/
#include <stddef.h>

seq_t a(idx_t n)
{
    /* ---------- Automaton construction (static init) ---------- */
    static int child[20][2];
    static int fail[20];
    static int trans[20][2];
    static int output[20];
    static size_t states;
    static int queue[20];
    static int qhead, qtail;
    static int initialized = 0;

    if (!initialized) {
        /* initialise structure */
        states = 1; /* root = 0 */
        for (int i = 0; i < 20; ++i) {
            child[i][0] = child[i][1] = -1;
            output[i] = 0;
            trans[i][0] = trans[i][1] = 0;
        }

        /* insert patterns */
        const char *pat[] = { "0000", "0101", "0111" };
        const int patcnt = sizeof(pat)/sizeof(pat[0]);

        for (int p = 0; p < patcnt; ++p) {
            int node = 0;
            for (const char *c = pat[p]; *c; ++c) {
                int b = *c - '0';
                if (child[node][b] == -1) {
                    child[node][b] = (int)states++;
                }
                node = child[node][b];
            }
            output[node] = 1;
        }

        /* build failure links using BFS */
        qhead = qtail = 0;
        for (int b = 0; b < 2; ++b) {
            int v = child[0][b];
            if (v != -1) {
                fail[v] = 0;
                queue[qtail++] = v;
            } else {
                child[0][b] = 0;
            }
        }

        while (qhead < qtail) {
            int r = queue[qhead++];
            output[r] = output[r] | output[fail[r]];
            for (int b = 0; b < 2; ++b) {
                int v = child[r][b];
                if (v != -1) {
                    fail[v] = child[fail[r]][b];
                    queue[qtail++] = v;
                } else {
                    child[r][b] = child[fail[r]][b];
                }
            }
        }

        /* transition table */
        for (size_t s = 0; s < states; ++s) {
            for (int b = 0; b < 2; ++b) {
                trans[s][b] = child[s][b];
            }
        }

        initialized = 1;
    }

    /* ---------- DP over automaton states ---------- */
    static seq_t cur[20];
    static seq_t nxt[20];

    for (size_t i = 0; i < states; ++i) cur[i] = 0;
    cur[0] = 1;

    for (idx_t pos = 0; pos < n; ++pos) {
        for (size_t i = 0; i < states; ++i) nxt[i] = 0;
        for (size_t s = 0; s < states; ++s) {
            if (cur[s] == 0) continue;
            if (output[s]) continue;
            seq_t val = cur[s];
            for (int b = 0; b < 2; ++b) {
                int ns = trans[s][b];
                if (output[ns]) continue;
                nxt[ns] += val;
            }
        }
        /* swap cur and nxt */
        for (size_t i = 0; i < states; ++i) cur[i] = nxt[i];
    }

    seq_t total = 0;
    for (size_t s = 0; s < states; ++s) {
        if (!output[s]) total += cur[s];
    }
    return total;
}
