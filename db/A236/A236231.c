/*
 * A236231  Number of binary words of length n with exactly 2 (possibly
 *          overlapping) occurrences of the subword given by the binary
 *          expansion of n.
 * https://oeis.org/A236231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of binary words of length n containing exactly two
 *            (possibly overlapping) occurrences of the binary expansion of n
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 35, u64 n <= 67, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a DP over the KMP automaton for the pattern P = bin(n).
   For each position we keep the number of strings that have seen exactly 0,
   1 or 2 occurrences, together with the current automaton state (how many
   initial characters of P are matched as the suffix of the built word). */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* a(0) = 0 by definition */

    /* ---- 1. Build pattern P = binary expansion of n (MSB -> LSB) ---- */
    int bits[64];
    int L = 0;                            /* length of P */
    idx_t tmp = n;
    while (tmp) {                         /* count bits */
        tmp >>= 1;
        L++;
    }
    for (int i = 0; i < L; i++) {
        bits[L - 1 - i] = (int)((n >> i) & 1ULL);  /* MSB at index 0 */
    }

    /* ---- 2. Compute failure function for KMP automaton ---- */
    int fail[64];
    fail[0] = 0;
    for (int i = 1; i < L; i++) {
        int k = fail[i - 1];
        while (k > 0 && bits[k] != bits[i]) k = fail[k - 1];
        if (bits[k] == bits[i]) k++;
        fail[i] = k;
    }
    /* fail[L-1] is the longest proper prefix of P which is also a suffix. */

    /* ---- 3. DP arrays: cur[match][state], nxt[match][state] ---- */
    seq_t cur[3][65] = {0};
    seq_t nxt[3][65] = {0};

    cur[0][0] = 1;   /* start with empty string, 0 matches, state 0 */

    for (idx_t pos = 0; pos < n; pos++) {
        /* reset nxt */
        for (int m = 0; m <= 2; m++)
            for (int s = 0; s <= L; s++)
                nxt[m][s] = 0;

        for (int m = 0; m <= 2; m++) {
            for (int s = 0; s <= L; s++) {
                seq_t val = cur[m][s];
                if (!val) continue;

                for (int bit = 0; bit <= 1; bit++) {
                    int k = s;
                    while (k > 0 && bits[k] != bit)
                        k = fail[k - 1];
                    if (bits[k] == bit) k++;
                    if (k == L) {                /* a full match appears */
                        if (m < 2) {
                            int m2 = m + 1;
                            int after = fail[L - 1];   /* next automaton state */
                            nxt[m2][after] += val;
                        }
                        /* if m==2 we would get >2 matches – discard */
                    } else {
                        nxt[m][k] += val;
                    }
                }
            }
        }

        /* swap cur and nxt */
        for (int m = 0; m <= 2; m++)
            for (int s = 0; s <= L; s++)
                cur[m][s] = nxt[m][s];
    }

    /* ---- 4. Sum over all states with exactly 2 matches ---- */
    seq_t res = 0;
    for (int s = 0; s <= L; s++) res += cur[2][s];
    return res;
}
