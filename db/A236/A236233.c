/*
 * A236233  Number of binary words of length n with exactly 4 (possibly
 *          overlapping) occurrences of the subword given by the binary
 *          expansion of n.
 * https://oeis.org/A236233
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of binary words of length n containing exactly
 *            four (possibly overlapping) occurrences of the binary expansion
 *            of n. Computed by dynamic programming on a finite‑state
 *            automaton (KMP)
 * Range:     u8 n <= 14, u16 n <= 26, u32 n <= 41, u64 n <= 72, u128 n <= 137,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define MAXL 64   /* largest pattern length for n < 2^64 */

static inline int floor_log2_uint64(idx_t n) {
    /* assumes n > 0 */
    return 63 - __builtin_clzll((unsigned long long)n);
}

/* Build the suffix automaton for pattern pat[0..L-1] (length L).
   trans[k][b] gives the length of the longest prefix of pat that is a
   suffix of pat[0..k-1] followed by bit b.  hit[k][b] is true iff this
   transition causes a full match of the pattern. */
static void build_automaton(int L, int *pat, int trans[MAXL][2],
                            bool hit[MAXL][2]) {
    int pi[MAXL];
    pi[0] = 0;
    for (int i = 1; i < L; ++i) {
        int j = pi[i - 1];
        while (j > 0 && pat[i] != pat[j]) j = pi[j - 1];
        if (pat[i] == pat[j]) ++j;
        pi[i] = j;
    }

    for (int k = 0; k < L; ++k) {
        for (int b = 0; b < 2; ++b) {
            int l = k;
            while (l > 0 && pat[l] != b) l = pi[l - 1];
            if (pat[l] == b) ++l;
            bool full = (l == L);
            if (full) l = pi[L - 1];          /* fallback after a match */
            trans[k][b] = l;
            hit[k][b] = full;
        }
    }
}

seq_t a(idx_t n) {
    if (n < 7) return 0;                  /* defined only for n >= 7 */

    int L = floor_log2_uint64(n) + 1;      /* pattern length */
    if (L > MAXL) return 0;               /* safety check */

    int pat[MAXL];
    for (int i = 0; i < L; ++i)
        pat[i] = (int)((n >> (L - 1 - i)) & 1);

    int trans[MAXL][2];
    bool hit[MAXL][2];
    build_automaton(L, pat, trans, hit);

    /* DP: cur[match_count][state] */
    seq_t cur[5][MAXL] = {0};
    seq_t nxt[5][MAXL] = {0};
    cur[0][0] = 1;

    for (idx_t pos = 0; pos < n; ++pos) {
        for (int m = 0; m <= 4; ++m)
            for (int s = 0; s < L; ++s)
                nxt[m][s] = 0;

        for (int m = 0; m <= 4; ++m) {
            for (int s = 0; s < L; ++s) {
                seq_t val = cur[m][s];
                if (!val) continue;
                for (int b = 0; b < 2; ++b) {
                    int ns = trans[s][b];
                    int nm = m + (hit[s][b] ? 1 : 0);
                    if (nm > 4) continue;
                    nxt[nm][ns] += val;
                }
            }
        }

        for (int m = 0; m <= 4; ++m)
            for (int s = 0; s < L; ++s)
                cur[m][s] = nxt[m][s];
    }

    seq_t res = 0;
    for (int s = 0; s < L; ++s)
        res += cur[4][s];
    return res;
}
