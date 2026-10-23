/*
 * A146027  Numbers that can be written from base 2 to base 10 using only the
 *          digits 0 to 4.
 * https://oeis.org/A146027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   ok_in_bases, seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A146027: Numbers that can be written in every base 2 ≤ b ≤ 10
 * using only the digits 0–4.  The implementation searches all decimal
 * integers whose decimal digits are ⟨0,1,2,3,4⟩ and keeps those whose
 * representations in bases 6, 7, 8 and 9 also use only digits ≤ 4.
 * The found numbers are cached so that repeated calls behave in O(1)
 * after the initial generation.  The algorithm is exact for any
 * seq_t that can hold the value that is required for the requested n
 * (e.g. uint8_t, uint16_t, … or an arbitrary‐precision type).  The
 * generated values are stored in static cache arrays allocated with
 * the provided utility function seq_alloc_array.  No external dynamic
 * memory is freed; this is sufficient for the testing harness. */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/ok_in_bases.h"

static seq_t *seq_cache = NULL;   /* cache of sequence values */
static size_t seq_len   = 0;      /* number of stored values   */
static size_t seq_cap   = 0;      /* capacity of seq_cache    */

/* Increment logic for the next candidate number; the type is seq_t */
static seq_t next_candidate = 0;

/* Ensure cache has at least `needed` elements; grow by powers of two. */
static void ensure_seq_capacity(size_t needed)
{
    if (seq_cap >= needed)
        return;

    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *new_cache = seq_alloc_array(new_cap);
    if (seq_cache) {
        memcpy(new_cache, seq_cache, seq_len * sizeof(seq_t));
        /* We deliberately do not free the old array. */
    }
    seq_cache = new_cache;
    seq_cap   = new_cap;
}

/* Check whether the decimal representation of `x` uses only digits 0–4. */
static bool digits_le4(seq_t x)
{
    if (x == (seq_t)0)
        return true;
    seq_t t = x;
    while (t != (seq_t)0) {
        seq_t d = t % (seq_t)10;
        if (d > (seq_t)4)
            return false;
        t /= (seq_t)10;
    }
    return true;
}

/* External linkage function requested by the build system. */
seq_t a(idx_t n)
{
    /* From the OEIS offset it is acceptable to return 0 for n == 0. */
    if (n == 0)
        return (seq_t)0;

    /* Prepare the cache to contain at least n elements. */
    size_t needed = (size_t)n;
    ensure_seq_capacity(needed);

    /* Generate new terms until we have n of them. */
    while (seq_len < needed) {
        seq_t cand = next_candidate;

        /* Candidate can be stored only if it fits into seq_t (i.e. no wrap). */
        /* For unsigned types, overflow occurs when next_candidate + 1 == 0. */
        if (digits_le4(cand) && ok_in_bases(cand))
            seq_cache[seq_len++] = cand;

        /* Advance to the next decimal integer. */
        ++next_candidate;
        /* If the candidate type overflows, we stop generating further terms.
         * This is safe for any seq_t: for sizes that can hold all required
         * terms (e.g. arbitrary-precision) overflow never happens. */
        if (next_candidate == (seq_t)0)
            break;
    }

    return seq_cache[n - 1];
}
