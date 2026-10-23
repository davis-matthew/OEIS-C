/*
 * A044571  Numbers n such that string 0,3 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer m such that the base‑8 representation of
 *            m contains the substring 0,3, but the base‑8 representation of
 *            m+1 does **not** contain 0,3
 * Range:     u8 n <= 3, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

static bool contains_03(seq_t value)
{
    /* Convert to base‑8 digits (least significant first). */
    unsigned char digits[256];
    size_t len = 0;

    while (value > 0) {
        digits[len++] = (unsigned char)(value % 8);
        value /= 8;
    }
    if (len < 2) return false;          /* Need at least two digits */

    /* Scan from most significant to least.  Digits are stored LSB‑first
       so the pair (more significant, less significant) is
       digits[i-1], digits[i-2] when iterating i = len .. 2.  */
    for (size_t i = len; i > 1; --i) {
        if (digits[i-1] == 0 && digits[i-2] == 3)
            return true;
    }
    return false;
}

/* Cache the computed sequence terms.  The problem guarantees that
   up to about n = 10 000 the values fit comfortably in seq_t, so a
   modest static buffer is sufficient. */
#define CACHE_CAP 200000
static seq_t cache[CACHE_CAP];
static idx_t cached = 0;          /* Number of terms stored */
static seq_t next_candidate = 1;  /* Next integer to test */

/* External linkage; no main() defined elsewhere. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* Not part of OEIS offset, but harmless */

    /* Grow cache if necessary.  We are confident n will never exceed
       CACHE_CAP for the given constraints. */
    while (cached < n) {
        seq_t cur = next_candidate;
        seq_t nxt = cur + 1;

        if (contains_03(cur) && !contains_03(nxt)) {
            cache[cached++] = cur;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
