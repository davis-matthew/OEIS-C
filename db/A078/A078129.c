/*
 * A078129  Numbers which cannot be written as sum of cubes > 1.
 * https://oeis.org/A078129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A078129: numbers that are NOT representable as a sum of
   cubes of integers greater than 1.  The sequence is finite; every
   integer > 181 can be expressed using only 8 (=2³) and 27 (=3³). */

/* This implementation builds the list of all non‑representable integers
   up to 181 once, then returns the(n‑th) element of that list.  For
   n greater than the sequence length the function returns 0. */

#include <stdbool.h>
#include <stddef.h>   /* for size_t */

#define MAX_VAL   181   /* all larger numbers are representable */
#define MAX_TERMS 200   /* safe upper bound on the number of terms */

/* Return the n-th element of the sequence (1‑based). */
seq_t a(idx_t n) {
    static bool initialized = false;
    static seq_t seq[MAX_TERMS];
    static size_t seq_len = 0;

    if (!initialized) {
        /* DP table: repr[i] == true if i can be written as sum of cubes
           of integers >= 2 and <= 5 (since 6³ = 216 > MAX_VAL). */
        bool repr[MAX_VAL + 1];
        repr[0] = true;
        for (idx_t i = 1; i <= MAX_VAL; ++i) {
            repr[i] = false;
        }

        /* Pre‑compute cubes up to MAX_VAL. */
        const idx_t cubes[] = {8, 27, 64, 125};
        const size_t num_cubes = sizeof(cubes) / sizeof(cubes[0]);

        for (size_t ci = 0; ci < num_cubes; ++ci) {
            idx_t c = cubes[ci];
            for (idx_t j = c; j <= MAX_VAL; ++j) {
                if (repr[j - c]) {
                    repr[j] = true;
                }
            }
        }

        /* Collect all numbers that are NOT representable. */
        seq_len = 0;
        for (idx_t i = 1; i <= MAX_VAL; ++i) {
            if (!repr[i]) {
                seq[seq_len++] = (seq_t)i;
            }
        }
        initialized = true;
    }

    if (n == 0 || (size_t)n > seq_len) {
        return (seq_t)0;   /* undefined for out-of‑range indices */
    }
    return seq[(size_t)n - 1];
}
