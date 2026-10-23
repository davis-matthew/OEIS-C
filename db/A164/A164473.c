/*
 * A164473  Number of binary strings of length n with no substrings equal to
 *          0001 0101 or 1011.
 * https://oeis.org/A164473
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 23, u32 n <= 49, u64 n <= 100,
 *            u128 n <= 203, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A164473: Number of binary strings of length n with no substrings
 * equal to 0001, 0101, or 1011.  The implementation builds a
 * deterministic automaton on the longest suffix of length ≤3 that is
 * a prefix of one of the forbidden patterns, then counts all
 * strings of a given length that never reach a forbidden state
 * (i.e. that never complete a forbidden length‑4 word). */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Forbidden patterns (length 4). */
static const char *forbidden[3] =
    { "0001", "0101", "1011" };

/*  Prefixes of length ≤3 that are prefixes of any forbidden pattern. */
static const char *prefixes[9] = {
    "",     /* 0: ""      */
    "0",    /* 1: "0"     */
    "00",   /* 2: "00"    */
    "000",  /* 3: "000"   */
    "01",   /* 4: "01"    */
    "010",  /* 5: "010"   */
    "1",    /* 6: "1"     */
    "10",   /* 7: "10"    */
    "101"   /* 8: "101"   */
};
static const int pref_len[9] = {0,1,2,3,2,3,1,2,3};

/* Transition table: state → bit → next state, -1 if forbidden. */
static int trans[9][2];
static bool trans_ready = false;

/* One‑time initialisation of the transition table. */
static void init_transitions(void)
{
    if (trans_ready) return;
    for (int s = 0; s < 9; ++s) {
        const char *sp = prefixes[s];
        int slen = pref_len[s];
        for (int bit = 0; bit <= 1; ++bit) {
            /* Build candidate string sp + bit */
            char tmp[6];                 /* max 3 + 1 + 1 (suffix) + null */
            memcpy(tmp, sp, slen);
            tmp[slen] = (char)('0' + bit);
            int tlen = slen + 1;
            tmp[tlen] = '\0';

            /* If the last 4 characters form a forbidden pattern,
             * this transition is illegal. */
            bool forbidden_transition = false;
            if (tlen >= 4) {
                const char *suffix4 = tmp + (tlen - 4);
                for (int f = 0; f < 3; ++f) {
                    if (memcmp(suffix4, forbidden[f], 4) == 0) {
                        forbidden_transition = true;
                        break;
                    }
                }
            }
            if (forbidden_transition) {
                trans[s][bit] = -1;
                continue;
            }

            /* Find longest suffix of tmp that is a prefix of a forbidden
             * pattern, of length ≤3. */
            int ns = -1;
            int max_consume = (tlen < 3 ? tlen : 3);
            for (int l = max_consume; l >= 0; --l) {
                const char *sub = tmp + (tlen - l);
                for (int p = 0; p < 9; ++p) {
                    if (pref_len[p] == l && memcmp(sub, prefixes[p], l) == 0) {
                        ns = p;
                        goto found;
                    }
                }
            }
found:
            trans[s][bit] = ns;   /* ns should never stay -1 here */
        }
    }
    trans_ready = true;
}

/* Memoised DP arrays */
static seq_t *dp = NULL;
static size_t dp_capacity = 0;   /* allocated size of dp array */
static size_t dp_len = 0;        /* number of already computed values */

/* Current and next counts for each state */
static seq_t cur_counts[9];
static seq_t next_counts[9];

/* Ensure that dp array can hold at least 'need' indices. */
static void ensure_dp_capacity(size_t need)
{
    if (dp_capacity > need) return;
    size_t new_cap = (dp_capacity == 0) ? 16 : dp_capacity * 2;
    while (new_cap <= need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
    if (!tmp) abort();  /* failure unlikely in contest environment */
    dp = tmp;
    dp_capacity = new_cap;
}

/* Build DP up to index 'n' (inclusive). */
static void build_up_to(idx_t n)
{
    init_transitions();

    if (dp_len == 0) {
        /* initialisation for length 0 */
        dp = (seq_t *)malloc(16 * sizeof(seq_t));
        if (!dp) abort();
        dp_capacity = 16;
        dp[0] = (seq_t)1;
        dp_len = 1;
        cur_counts[0] = (seq_t)1;
        for (int i = 1; i < 9; ++i) cur_counts[i] = (seq_t)0;
    }

    while (dp_len <= n) {
        /* zero next_counts */
        for (int i = 0; i < 9; ++i) next_counts[i] = (seq_t)0;

        /* transition */
        for (int s = 0; s < 9; ++s) {
            if (cur_counts[s] == (seq_t)0) continue;
            seq_t cur_val = cur_counts[s];
            for (int bit = 0; bit <= 1; ++bit) {
                int ns = trans[s][bit];
                if (ns < 0) continue;    /* forbidden transition */
                next_counts[ns] = next_counts[ns] + cur_val;
            }
        }

        /* sum up and store result for length dp_len */
        seq_t total = (seq_t)0;
        for (int i = 0; i < 9; ++i) total = total + next_counts[i];
        ensure_dp_capacity(dp_len);
        dp[dp_len] = total;

        /* prepare for next iteration */
        for (int i = 0; i < 9; ++i) cur_counts[i] = next_counts[i];
        ++dp_len;
    }
}

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 4.  For n < 4 we return 1
     * (undefined behaviour is not required). */
    if (n < 4) return (seq_t)1;
    build_up_to(n);
    return dp[n];
}
