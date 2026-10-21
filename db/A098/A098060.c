/*
 * A098060  Numbers whose French names end in a vowel.
 * https://oeis.org/A098060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  word, nonn
 * Formula:   a(n) = the nth non‑negative integer whose French name ends in a
 *            vowel. Computed by searching numbers incrementally and testing
 *            the last letter of the French representation
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   ends_with_vowel, last_french_letter, last_letter_under_20
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ends_with_vowel.h"
#include "../utils/last_french_letter.h"
#include "../utils/last_letter_under_20.h"

/* ---------- sequence storage and construction ---------- */

static seq_t *seq_cache = NULL;
static size_t seq_len   = 0;
static size_t seq_cap   = 0;
static uint64_t next_check = 0;   /* next integer to test */

/* grow the cache */
static void ensure_capacity(size_t needed)
{
    if (seq_cap >= needed)
        return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 64;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_cache = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap   = new_cap;
}

/* ---------- required function ---------- */

seq_t a(idx_t n)
{
    if (n == 0)               /* offset is 1, but guard against 0 */
        return 0;

    while (seq_len < (size_t)n) {
        /* find the next integer with a vowel ending */
        while (!ends_with_vowel(next_check))
            next_check++;

        ensure_capacity(seq_len + 1);
        seq_cache[seq_len++] = (seq_t)next_check;
        next_check++;          /* move to the next candidate */
    }
    return seq_cache[n - 1];
}
