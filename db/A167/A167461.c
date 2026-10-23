/*
 * A167461  Anagram multiples of 123456789.
 * https://oeis.org/A167461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function enumerates multiples of 123456789 that are 10–digit permutations
   of the digits 0–9 (i.e. full six‑digit distinct digit numbers).  There are
   exactly 27 such numbers, and the function returns the n-th smallest of
   them for n >= 1.  If n is larger than 27 it returns 0. */
#include "../utils/digit_counts.h"
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache to avoid repeating the search for every call */
    static seq_t cache[81];
    static idx_t count = 0;
    static bool built = false;

    if (!built) {
        built = true;
        const seq_t MIN10 = (seq_t)1000000000ULL;
        const seq_t MAX10 = (seq_t)10000000000ULL;  /* exclusive upper bound */

        for (idx_t k = 1; k <= 81; ++k) {
            seq_t prod = (seq_t)k * (seq_t)123456789ULL;

            if (prod < MIN10) continue;
            if (prod >= MAX10) break;  /* safety for non‑64‑bit seq_t */

            unsigned char cnt[10] = {0};
            digit_counts(prod, cnt);

            bool ok = true;
            for (int d = 0; d < 10; ++d) {
                if (cnt[d] != 1) { ok = false; break; }
            }

            if (ok) {
                cache[count++] = prod;
            }
        }
    }

    if (n - 1 < count)
        return cache[n - 1];
    else
        return (seq_t)0;
}
