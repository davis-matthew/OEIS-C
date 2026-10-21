/*
 * A236235  Number of binary words of length n with exactly 6 (possibly
 *          overlapping) occurrences of the subword given by the binary
 *          expansion of n.
 * https://oeis.org/A236235
 *
 * Offset:    15    (a(15) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 30, u32 n <= 44, u64 n <= 81, u128 n <= 145,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) = number of binary words of length n that contain
 * exactly 6 (possibly overlapping) occurrences of the binary representation
 * of n.  The value is computed by dynamic programming over the
 * automaton for the pattern, using the prefix‑function (KMP).  The
 * function works for any seq_t and idx_t supplied by the build system.
 * For n < 15 the result is 0, as the offset of the OEIS sequence is 15. */

#include <stdint.h>
#include <stdbool.h>

/* helper to compute the bit length of n (n > 0) */
static inline int bitlen_idx(uint64_t n) {
    /* GCC/Clang built‑in __builtin_clzll works for unsigned long long */
    return 64 - __builtin_clzll((unsigned long long)n);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Sequence is defined only for n >= 15 */
    if (n < 15) return 0;

    /* Length of the binary pattern (number of bits of n). */
    const int m = bitlen_idx((uint64_t)n);          /* 1 <= m <= 64 for n >= 1 */
    const int MAXK = 6;                              /* count of required occurrences */

    /* ---- Pattern extraction ---- */
    int pattern[64];  /* pattern bits, MSB first */
    for (int i = 0; i < m; ++i) {
        pattern[i] = (int)((n >> (m - 1 - i)) & 1ULL);
    }

    /* ---- Prefix function (KMP failure array) ---- */
    int pi[64];
    pi[0] = 0;
    for (int i = 1; i < m; ++i) {
        int k = pi[i - 1];
        while (k > 0 && pattern[i] != pattern[k]) k = pi[k - 1];
        if (pattern[i] == pattern[k]) ++k;
        pi[i] = k;
    }

    /* ---- Transition and occurrence arrays ---- */
    int next_state[64][2];
    int inc[64][2];
    for (int s = 0; s < m; ++s) {
        for (int b = 0; b < 2; ++b) {
            int t = s;
            while (t > 0 && pattern[t] != b) t = pi[t - 1];
            if (pattern[t] == b) ++t;
            if (t == m) {                  /* pattern complete */
                inc[s][b] = 1;              /* one occurrence counted */
                t = pi[m - 1];              /* continue with longest prefix */
            } else {
                inc[s][b] = 0;
            }
            next_state[s][b] = t;
        }
    }

    /* ---- Dynamic programming over positions ---- */
    seq_t curr[64][7] = {0};
    seq_t nextdp[64][7] = {0};
    curr[0][0] = 1;  /* start with empty word, no matches yet */

    for (idx_t pos = 0; pos < n; ++pos) {
        /* reset nextdp to zero */
        for (int s = 0; s < m; ++s)
            for (int k = 0; k <= MAXK; ++k)
                nextdp[s][k] = 0;

        for (int s = 0; s < m; ++s) {
            for (int k = 0; k <= MAXK; ++k) {
                seq_t ways = curr[s][k];
                if (ways == 0) continue;
                for (int b = 0; b < 2; ++b) {
                    int ns = next_state[s][b];
                    int nk = k + inc[s][b];
                    if (nk > MAXK) continue;
                    nextdp[ns][nk] += ways;
                }
            }
        }

        /* swap curr and nextdp */
        for (int s = 0; s < m; ++s)
            for (int k = 0; k <= MAXK; ++k)
                curr[s][k] = nextdp[s][k];
    }

    /* ---- Sum in states with exactly 6 occurrences ---- */
    seq_t result = 0;
    for (int s = 0; s < m; ++s)
        result += curr[s][MAXK];

    return result;
}
