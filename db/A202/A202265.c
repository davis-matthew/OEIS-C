/*
 * A202265  Nonprime numbers in which all substrings and reversal substrings
 *          are nonprimes.
 * https://oeis.org/A202265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) computed by searching for the n‑th nonprime number whose
 *            every substring and every reverse substring is nonprime. The
 *            search is performed in increasing numeric order by generating
 *            all valid numbers of length 1, then all of length 2, and so on.
 *            The implementation uses depth‑first search for e...
 * Range:     u8 n <= 28, u16 n <= 469, u32 n <= 469, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

/* Maximum number of digits we are willing to handle.
   For the typical test cases (n ≤ 10000) the 10000‑th term
   is far below 200 digits, so this is safe for any
   built seq_t that can hold that many digits. */
#define MAX_DEPTH 200

/* Allowed digits (most‑significant first).  The digit '0' is
   permitted only after the first position. */
static const seq_t ALLOWED[6] = {0, 1, 4, 6, 8, 9};

/* Global state for the DFS.  All arrays are indexed by depth
   and suffix length (1 ≤ len ≤ depth).  The entries at
   depth 0 are not used. */
static seq_t S[MAX_DEPTH + 1][MAX_DEPTH + 1];
static seq_t R[MAX_DEPTH + 1][MAX_DEPTH + 1];
static seq_t POW10[MAX_DEPTH + 1];

/* Result for the current query. */
static seq_t  ANSWER;
static idx_t  TARGET_N;
static bool   FOUND;
static idx_t  SEEN /* count of terms found so far, including the 0 term */;

/* Record a candidate number that satisfies the property.
   Once the TARGET_N'th term is found, store it in ANSWER. */
static void record_value(seq_t v)
{
    SEEN++;
    if (SEEN == TARGET_N) {
        ANSWER = v;
        FOUND = true;
    }
}

/* Depth‑first search for numbers of exact length L.
   depth   – number of digits already chosen (0 … L-1)
   val     – numeric value of those digits */
static void dfs_len(idx_t target_len, int depth, seq_t val)
{
    if (FOUND) return;

    if (depth == target_len) {
        record_value(val);
        return;
    }

    /* Iterate over all allowed digits.  Skip leading zero. */
    for (int i = 0; i < 6 && !FOUND; ++i) {
        seq_t d = ALLOWED[i];
        if (depth == 0 && d == 0) continue;

        seq_t new_val = val * (seq_t)10 + d;

        /* Single‑digit substring must be nonprime. */
        if (is_prime_big(d)) continue;

        /* Prepare arrays for the new depth. */
        seq_t *S_cur = S[depth + 1];
        seq_t *R_cur = R[depth + 1];

        S_cur[1] = d;
        R_cur[1] = d;

        /* Pow10[depth+1] is needed for deeper levels. */
        POW10[depth + 1] = POW10[depth] * (seq_t)10;

        bool ok = true;

        /* Extend all existing suffixes. */
        for (int l = 1; l <= depth; ++l) {
            seq_t new_suf = S[depth][l] * (seq_t)10 + d;
            if (is_prime_big(new_suf)) { ok = false; break; }
            seq_t new_rev = d * POW10[l] + R[depth][l];
            if (is_prime_big(new_rev)) { ok = false; break; }
            S_cur[l + 1] = new_suf;
            R_cur[l + 1] = new_rev;
        }

        if (!ok) continue;

        dfs_len(target_len, depth + 1, new_val);
    }
}

/* Main function requested by the framework. */
seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)0;  /* a(1) = 0 */

    TARGET_N = n;
    FOUND    = false;
    ANSWER   = (seq_t)0;
    SEEN     = (idx_t)1;                 /* zero counted as first term */

    POW10[0] = (seq_t)1;

    for (int len = 1; len <= MAX_DEPTH && !FOUND; ++len) {
        dfs_len((idx_t)len, 0, (seq_t)0);
    }

    return ANSWER;
}
