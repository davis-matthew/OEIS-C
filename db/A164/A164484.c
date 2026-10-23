/*
 * A164484  Number of binary strings of length n with no substrings equal to
 *          0001 0111 or 1100.
 * https://oeis.org/A164484
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 24, u32 n <= 50, u64 n <= 103,
 *            u128 n <= 209, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: uses a DFA on the last three bits of the string.  
   For each length n the number of allowed binary strings is the sum
   of the counts of all eight possible 3‑bit suffixes.  The dynamic
   program runs in O(n) time and O(1) additional space. */

#include <stdlib.h>   /* malloc, calloc, realloc, free */
#include <stdbool.h>  /* bool */
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

#ifdef __cplusplus
/* In C++ use operator new/delete to allocate arrays of class objects. */

#else
/* In C use malloc/calloc. */

#endif

/* Cache of sequence values.  tot[ n ] = a(n).  tot[0]..tot[ready] are ready. */
static seq_t *tot = NULL;
static size_t tot_cap = 0;
static size_t ready = 0;

/* Current and next DP state arrays: dp[ cur ][ s ] = #strings of current length
   ending with suffix s (0..7).  We maintain two rows and swap after each step. */
static seq_t dp[2][8];
static size_t cur = 0, nxt = 1;

/* Ensure that tot[0..n] are filled.  Called by a(). */
static void ensure(idx_t n)
{
    size_t needed = (size_t)n;
    /* Initialisation (only once). */
    if (tot == NULL) {
        /* Allocate for indices 0..3 (four elements). */
        tot_cap = 4;
        tot = alloc_seq_array(tot_cap);
        if (tot == NULL) return;  /* allocation failed – future calls will return 0 */

        tot[0] = (seq_t)1;  /* length 0 : one empty string */
        tot[1] = (seq_t)2;  /* length 1 : {0,1} */
        tot[2] = (seq_t)4;  /* length 2 : all 4 strings */
        tot[3] = (seq_t)8;  /* length 3 : all 8 strings */

        /* Initialise DP for length 3: each 3‑bit suffix occurs once. */
        for (int s = 0; s < 8; ++s) dp[0][s] = (seq_t)1;
        for (int s = 0; s < 8; ++s) dp[1][s] = (seq_t)0;

        ready = 3;  /* we have values up to tot[3] */
    }

    if (needed <= ready) return;           /* already computed */

    /* Grow tot array if necessary. */
    if (needed + 1 > tot_cap) {
        size_t new_cap = needed + 1;
        seq_t *new_tot;
        /* Reallocate using the allocator defined above. */
        new_tot = alloc_seq_array(new_cap);
        if (new_tot == NULL) return;      /* allocation failed */

        /* Copy old contents. */
        for (size_t i = 0; i < tot_cap; ++i) new_tot[i] = tot[i];
        /* Free old storage. */
        free_seq_array(tot);

        tot = new_tot;
        tot_cap = new_cap;
    }

    /* DP transition: compute tot[ready+1]..tot[needed] */
    for (size_t len = ready + 1; len <= needed; ++len) {
        /* Zero next row. */
        for (int s = 0; s < 8; ++s) dp[nxt][s] = (seq_t)0;

        /* From each current state, try appending 0 or 1. */
        for (int s = 0; s < 8; ++s) {
            seq_t count = dp[cur][s];
            if (count == (seq_t)0) continue;
            for (int b = 0; b < 2; ++b) {
                int pat = (s << 1) | b;       /* candidate 4‑bit substring */
                if (pat == 1 || pat == 7 || pat == 12) continue; /* forbidden */
                int ns = pat & 7;              /* new 3‑bit suffix */
                dp[nxt][ns] += count;
            }
        }

        /* Sum all states to get total for this length. */
        seq_t sum = (seq_t)0;
        for (int s = 0; s < 8; ++s) sum += dp[nxt][s];
        tot[len] = sum;

        /* Swap current/next. */
        size_t tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    ready = needed;
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    if (n < 4) {
        /* For n = 0..3 the full set of binary strings is allowed. */
        return (seq_t)1 << (size_t)n;
    }

    ensure(n);
    return tot[(size_t)n];
}
