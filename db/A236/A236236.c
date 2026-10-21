/*
 * A236236  Number of binary words of length n with exactly 7 (possibly
 *          overlapping) occurrences of the subword given by the binary
 *          expansion of n.
 * https://oeis.org/A236236
 *
 * Offset:    15    (a(15) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 30, u32 n <= 50, u64 n <= 84, u128 n <= 145,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A236236: a(n) = number of binary words of length n that contain
   exactly 7 (possibly overlapping) occurrences of the binary representation
   of n.  Offset: 15.  The implementation uses a KMP‑style automaton and
   dynamic programming over the word positions. */

#include <stdbool.h>

static bool      inc_occ[16][2];   /* pattern matched in transition?   */
static int       nxt_state[16][2]; /* next automaton state            */
static seq_t     cur[200], nxt[200]; /* DP tables (states <= 14*8 = 112)  */

seq_t a(idx_t n)
{
    if (n < 15) return 0; /* value for indices below the offset is irrelevant */

    /* ---------- build the binary pattern of n (most significant bit first) ---------- */
    int bits[16];           /* binary representation of n, bits[0] = MSB   */
    idx_t t = n;
    int L = 0;
    /* temporary storage reverse order */
    int rev[16];
    while (t) {
        rev[L++] = (int)(t & 1ULL);
        t >>= 1;
    }
    for (int i = 0; i < L; ++i)
        bits[i] = rev[L - 1 - i];

    /* ---------- compute prefix function pi for KMP ---------- */
    int pi[16] = {0};
    for (int i = 1; i < L; ++i) {
        int j = pi[i - 1];
        while (j > 0 && bits[i] != bits[j]) j = pi[j - 1];
        if (bits[i] == bits[j]) ++j;
        pi[i] = j;
    }

    /* ---------- build transition table and occurrence flags ---------- */
    for (int s = 0; s < L; ++s) {
        for (int b = 0; b < 2; ++b) {
            int cur_t = s;
            while (cur_t > 0 && bits[cur_t] != b) cur_t = pi[cur_t - 1];
            if (bits[cur_t] == b) ++cur_t;
            if (cur_t == L) {                  /* full match */
                inc_occ[s][b] = true;
                nxt_state[s][b] = pi[L - 1];   /* fallback after match */
            } else {
                inc_occ[s][b] = false;
                nxt_state[s][b] = cur_t;
            }
        }
    }

    /* ---------- DP: initialise ---------- */
    int states = L * 8;                      /* 0..L-1 and 0..7 occurrences */
    for (int i = 0; i < states; ++i) cur[i] = 0;
    cur[0] = 1;                               /* 0 occurrences, state 0 */

    /* ---------- DP over all positions of the word ---------- */
    for (idx_t pos = 0; pos < n; ++pos) {
        /* zero next table */
        for (int i = 0; i < states; ++i) nxt[i] = 0;

        for (int s = 0; s < L; ++s) {
            for (int occ = 0; occ <= 7; ++occ) {
                seq_t val = cur[s * 8 + occ];
                if (!val) continue;

                /* try next bit 0 */
                int ns = nxt_state[s][0];
                bool inc = inc_occ[s][0];
                int occ2 = occ + (inc ? 1 : 0);
                if (occ2 <= 7) nxt[ns * 8 + occ2] += val;

                /* try next bit 1 */
                ns = nxt_state[s][1];
                inc = inc_occ[s][1];
                occ2 = occ + (inc ? 1 : 0);
                if (occ2 <= 7) nxt[ns * 8 + occ2] += val;
            }
        }

        /* move to next step */
        for (int i = 0; i < states; ++i) cur[i] = nxt[i];
    }

    /* ---------- sum over final states with exactly 7 occurrences ---------- */
    seq_t result = 0;
    for (int s = 0; s < L; ++s)
        result += cur[s * 8 + 7];

    return result;
}
