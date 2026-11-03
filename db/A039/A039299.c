/*
 * A039299  Numbers whose base-6 representation has the same nonzero number of
 *          1's and 3's.
 * https://oeis.org/A039299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth positive integer whose base‑6 representation
 *            contains the same non‑zero number of digit 1's and digit 3's.
 *            The sequence starts 9, 19, 39, … as given in the OEIS entry.
 *            This implementation lazily generates terms up to the required
 *            index, storing them in a static array for subsequent...
 * Range:     u8 n <= 32, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 70 terms through n = 857 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true if x has the same number of 1’s and 3’s in base‑6
   and that count is non‑zero. */
static bool has_equal_13(seq_t x)
{
    unsigned count1 = 0, count3 = 0;
    while (x > 0) {
        unsigned digit = (unsigned)(x % 6);
        if (digit == 1) ++count1;
        else if (digit == 3) ++count3;
        x /= 6;
    }
    return (count1 > 0) && (count1 == count3);
}

static seq_t *terms = NULL;      /* stored sequence values          */
static size_t terms_cap = 0;      /* capacity of the terms array     */
static size_t terms_len = 0;      /* number of terms currently stored*/
static seq_t last_checked = 0;    /* largest number that has been tested */

/* Ensure that the array contains at least 'need' terms. */
static void ensure_terms(size_t need)
{
    if (terms_len >= need) return;

    /* Grow the array as necessary. */
    if (terms_cap == 0) {
        terms_cap = 8;
        terms = mem_new(seq_t, terms_cap);
    }
    while (terms_len < need) {
        /* Search for the next good number. */
        do { ++last_checked; } while (!has_equal_13(last_checked));

        /* Append to the array. */
        if (terms_len == terms_cap) {
            terms_cap *= 2;
            terms = mem_resize(seq_t, terms, terms_cap);
        }
        terms[terms_len++] = last_checked;
    }
}

seq_t a(idx_t n)
{
    /* OEIS offset is 1, so ignore n == 0. */
    if (n < 1) return 0;

    ensure_terms((size_t)n);
    return terms[n - 1];
}
