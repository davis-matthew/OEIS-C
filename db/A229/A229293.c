/*
 * A229293  Number of binary words of length n with exactly k (possibly
 *          overlapping) occurrences of the subword given by the binary
 *          expansion of n for maximal k with at least one word.
 * https://oeis.org/A229293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of binary words of length n that contain the *
 *            maximum possible number of overlapping occurrences of the binary
 *            * representation of n. Computed by DP on the automaton for the
 *            single * pattern (KMP failure function)
 * Range:     u8 n <= 23, u16 n <= 95, u32 n <= 295, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Length of the pattern (binary expansion of n).  For n = 0 the
       pattern is the single digit "0". */
    idx_t plen = 0;
    for (idx_t t = n; t > 0; ++plen, t >>= 1);
    if (plen == 0) plen = 1;                 /* n = 0 */
    int L = (int)plen;                       /* small integer for arrays */

    /* Bit representation of n, most significant first. */
    int bits[64];
    if (n == 0) {
        bits[0] = 0;
    } else {
        for (int i = 0; i < L; ++i) {
            bits[i] = (int)((n >> (L - 1 - i)) & 1ULL);
        }
    }

    /* KMP failure function (lps array). */
    int lps[64];
    lps[0] = 0;
    for (int i = 1; i < L; ++i) {
        int j = lps[i - 1];
        while (j > 0 && bits[i] != bits[j]) j = lps[j - 1];
        if (bits[i] == bits[j]) ++j;
        lps[i] = j;
    }

    /* Transition table and occurrence increment. */
    int nxt[64][2];
    int inc[64][2];
    for (int s = 0; s < L; ++s) {
        for (int b = 0; b <= 1; ++b) {
            int j = s;
            while (j > 0 && bits[j] != b) j = lps[j - 1];
            if (bits[j] == b) ++j;
            if (j == L) {                    /* full pattern matched */
                inc[s][b] = 1;
                j = lps[L - 1];
            } else {
                inc[s][b] = 0;
            }
            nxt[s][b] = j;
        }
    }

    /* DP states: for each automaton state keep the maximum number of
       occurrences seen so far and the count of words achieving that
       maximum. */
    struct State { idx_t occ; seq_t count; };
    struct State cur[64], next[64];
    const idx_t SENTINEL = (idx_t)(-1);
    for (int s = 0; s < L; ++s) {
        cur[s].occ  = SENTINEL;
        cur[s].count = 0;
    }
    cur[0].occ  = 0;
    cur[0].count = 1;

    for (idx_t pos = 0; pos < n; ++pos) {
        for (int s = 0; s < L; ++s) {
            next[s].occ  = SENTINEL;
            next[s].count = 0;
        }
        for (int s = 0; s < L; ++s) {
            if (cur[s].occ == SENTINEL) continue;
            for (int b = 0; b <= 1; ++b) {
                int ns = nxt[s][b];
                idx_t new_occ = cur[s].occ + (idx_t)inc[s][b];
                if (next[ns].occ == SENTINEL || new_occ > next[ns].occ) {
                    next[ns].occ  = new_occ;
                    next[ns].count = cur[s].count;
                } else if (new_occ == next[ns].occ) {
                    next[ns].count += cur[s].count;
                }
            }
        }
        for (int s = 0; s < L; ++s) cur[s] = next[s];
    }

    /* Find the maximum number of occurrences and the total number of words
       achieving it. */
    idx_t best_occ = 0;
    seq_t result = 0;
    int first = 1;
    for (int s = 0; s < L; ++s) {
        if (cur[s].occ == SENTINEL) continue;
        if (first || cur[s].occ > best_occ) {
            best_occ = cur[s].occ;
            result   = cur[s].count;
            first = 0;
        } else if (cur[s].occ == best_occ) {
            result += cur[s].count;
        }
    }
    return result;
}
