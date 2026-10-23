/*
 * A145116  Numbers of length n binary words with fewer than 8 0-digits
 *          between any pair of consecutive 1-digits.
 * https://oeis.org/A145116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A145116: a(n) = number of binary words of length n with fewer than 8
   0‑digits between any pair of consecutive 1‑digits.  We use a dynamic-state
   DP with 10 states:
     0 – no previous 1 (leading zeros allowed),
     1 – last symbol was 1 (0 zeros since last 1),
     2–8 – last symbol was 0 and there are 1–7 zeros since the last 1,
     9 – last symbol was 0 and there are ≥8 zeros since the last 1 (a 1 is forbidden).
   The transition table is applied for each new position.  The result for a
   given n is the sum of the 10 state counts after n steps. */

#include <stdlib.h>
#include <stdint.h>

/*--- Cache for computed values -------------------------------------------*/
#define MAX_CACHE_N 100000          /* enough for n up to ~10 000                */
static seq_t cache[MAX_CACHE_N];   /* memoised a(n) values   */
static idx_t cache_len;            /* number of already computed entries (n+1) */

/*--- Current state distribution ------------------------------------------*/
static seq_t cur_state[10];

/* Initialise the cache and the state arrays (first call only). */
static void init(void)
{
    cache[0] = (seq_t)1;        /* a(0) = 1 */
    cache_len = 1;              /* we have a(0) */
    for (int i = 0; i < 10; ++i) cur_state[i] = (seq_t)0;
    cur_state[0] = (seq_t)1;    /* state 0 (no previous 1) for length 0      */
}

/* Compute a(n) for any n >= 0.  Calls are memoised and can come in any order. */
seq_t a(idx_t n)
{
    /* Initialise on the first call. */
    if (cache_len == 0) init();

    /* Return cached value if already known. */
    if (n < cache_len) return cache[(size_t)n];

    /* Ensure we never exceed the pre‑allocated cache. */
    if (n >= MAX_CACHE_N) {
        /* If the pre‑allocated space is insufficient, abort – it is
           guaranteed by the problem statement that n ≤ 10 000. */
        abort();
    }

    /* Extend the cache up to n.  We already have all values up to cache_len−1.
       We iterate from length cache_len to n, updating cur_state each time. */
    for (idx_t len = cache_len; len <= n; ++len) {
        /* next_state will hold the distribution for length len. */
        seq_t next_state[10] = {0};

        /* Transition from state 0 (no previous 1). */
        next_state[0] += cur_state[0];          /* add 0 */
        next_state[1] += cur_state[0];          /* add 1 */

        /* Transition from state 1 (last symbol was 1, zeros_since=0). */
        next_state[2] += cur_state[1];          /* add 0 → zeros_since=1 */
        next_state[1] += cur_state[1];          /* add 1 → stay in state 1 */

        /* Transitions for states 2–8 (1–7 zeros since last 1). */
        for (int s = 2; s <= 8; ++s) {
            next_state[s + 1] += cur_state[s];   /* add 0 → increase zeros count */
            next_state[1]     += cur_state[s];   /* add 1 → reset zero count */
        }

        /* Transition from state 9 (≥8 zeros since last 1: cannot place a 1). */
        next_state[9] += cur_state[9];          /* add 0 only */

        /* Copy next_state into cur_state for the next iteration. */
        for (int i = 0; i < 10; ++i) cur_state[i] = next_state[i];

        /* Compute total number of words of length len. */
        seq_t total = cur_state[0];
        for (int i = 1; i < 10; ++i) total += cur_state[i];

        cache[(size_t)len] = total;
    }

    /* At this point cache_len == n+1. */
    cache_len = n + 1;
    return cache[(size_t)n];
}
