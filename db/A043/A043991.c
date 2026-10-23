/*
 * A043991  Numbers k such that 5 and 7 occur juxtaposed in the base-9
 *          representation of k but not of k+1.
 * https://oeis.org/A043991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Numbers k such that the digits 5 and 7 occur adjacently * (in
 *            either order) in the base‑9 representation of k but not in k+1.
 *            * Computed by linear search with memoisation
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached sequence values */
static seq_t *cache   = NULL;
/* Number of terms stored in the cache */
static size_t len     = 0;
/* Allocated cache capacity */
static size_t cap     = 0;
/* Next integer to test (idx_t is unsigned) */
static idx_t next_k   = 1;

/* Return 1 if base‑9 representation of x contains the substring "57" or "75". */
static int has57(seq_t x)
{
    seq_t prev = 0;
    int   used = 0;          /* at least one digit has been seen */

    while (x > 0) {
        seq_t d = x % 9;          /* least significant base‑9 digit */
        if (used && ((prev == 5 && d == 7) || (prev == 7 && d == 5)))
            return 1;
        prev = d;
        used = 1;
        x = x / 9;
    }
    return 0;
}

/* Return the n‑th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    while (len < n) {
        /* Check the current candidate */
        if (has57((seq_t)next_k) && !has57((seq_t)(next_k + 1))) {
            /* Grow cache if necessary */
            if (len == cap) {
                size_t new_cap = (cap == 0) ? 128 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;   /* allocation failure */
                cache = tmp;
                cap   = new_cap;
            }
            cache[len++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return cache[n - 1];
}
