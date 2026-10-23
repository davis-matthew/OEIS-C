/*
 * A043768  Numbers k such that number of runs in the base 2 representation of
 *          k is congruent to 5 mod 10.
 * https://oeis.org/A043768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043768.
   a(n) = the nth positive integer whose binary representation has
   a number of runs ≡ 5 (mod 10).  A run is a maximal block of equal
   consecutive bits.  The function works for any build: 64‑bit
   integers, larger integer types, or an arbitrary‑precision class. */

#include <stddef.h>

/* The function signature is supplied by the build system. */
seq_t a(idx_t n)
{
    /* A small static cache.  For n ≤ 10 000 the first 200 000 integers
       are sufficient – the density of suitable numbers is roughly
       1/10.  Adjust MAX_CACHE if larger indices are required. */
    #define MAX_CACHE 200000
    static seq_t cache[MAX_CACHE];
    static idx_t filled = 0;          /* how many entries are stored */
    static seq_t next_candidate = 1;  /* next integer to test */

    while (filled < n) {
        seq_t temp = next_candidate;
        int runs = 0;
        int prev = -1;  /* -1 means "no previous bit" */

        /* Count runs in the binary form of temp. */
        while (temp > 0) {
            int bit = (temp & (seq_t)1) ? 1 : 0;
            if (bit != prev) {
                ++runs;
                prev = bit;
            }
            temp >>= 1;
        }

        if (runs % 10 == 5) {
            /* Store the valid candidate. */
            if (filled < MAX_CACHE) {
                cache[filled] = next_candidate;
                ++filled;
            } else {
                /* Requested index exceeds pre‑allocated cache.
                   In the context of the problem this should never
                   happen for n ≤ 10 000. */
                /* Fallback: return zero (should not occur). */
                return (seq_t)0;
            }
        }

        ++next_candidate;
    }

    return cache[n - 1];
}
