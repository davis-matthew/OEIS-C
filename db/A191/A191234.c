/*
 * A191234  The number of strong binary words of length n.
 * https://oeis.org/A191234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A191234 – Number of strong binary words of length n
    The sequence is zero for n > 37.
    The algorithm enumerates all binary words of length ≤ 37,
    pruning when the provisional number of proven square starts
    exceeds the allowed threshold.
    It records the count of strong words for each length once,
    then returns the pre‑computed value.  */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define MAXLEN 37

static seq_t counts[MAXLEN + 1] = {0};
static bool ready = false;
static uint8_t bits[MAXLEN + 1];
static bool proven[MAXLEN + 1];

/*  Recursive enumeration of all binary words of length ≤ MAXLEN.
    When a word of length `len` satisfies the strong condition it
    is counted in `counts[len]`.  */
static void dfs(int len) {
    /* Re‑compute all proven square starts with the current word. */
    for (int i = 1; i <= len; ++i) {
        proven[i] = false;
        int maxk = (len - i + 1) / 2;
        for (int k = 1; k <= maxk; ++k) {
            bool same = true;
            for (int t = 0; t < k; ++t) {
                if (bits[i + t] != bits[i + k + t]) {
                    same = false;
                    break;
                }
            }
            if (same) {
                proven[i] = true;
                break;
            }
        }
    }

    /* Quick pruning: if any prefix already has too many proven
       square starts, abandon this branch. */
    int cum = 0;
    for (int L = 1; L <= len; ++L) {
        if (proven[L]) ++cum;
        if (cum > L / 2) return;           /* prune */
    }

    /* The current word of length `len` is strong.  */
    ++counts[(size_t)len];

    /* Extend the word if we have not yet reached the maximum length. */
    if (len == MAXLEN) return;
    bits[len + 1] = 0;
    dfs(len + 1);
    bits[len + 1] = 1;
    dfs(len + 1);
}

/* Public interface required by the build. */
seq_t a(idx_t n) {
    if (!ready) {
        /* Initialise bits array (not strictly needed). */
        for (int i = 0; i <= MAXLEN; ++i) bits[i] = 0;
        /* Enumerate words starting with 0. */
        bits[1] = 0;
        dfs(1);
        /* Enumerate words starting with 1. */
        bits[1] = 1;
        dfs(1);
        ready = true;
    }
    if (n == 0 || n > MAXLEN) return (seq_t)0;
    return counts[(size_t)n];
}
