/*
 * A044647  Numbers k such that string 1,6 occurs in the base 9 representation
 *          of k but not of k+1.
 * https://oeis.org/A044647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044647:
   numbers k such that the base‑9 representation of k contains the
   substring "1 6" (adjacent digits 1 followed by 6) but the base‑9
   representation of k+1 does not.  The routine computes terms by
   brute‑force scanning; it keeps a cached list of found terms so that
   arbitrary order of calls is supported. */

#include <stdlib.h>
#include <stdbool.h>

/* Return true iff the base‑9 representation of x contains the digits
   1 followed immediately by 6 (with 1 more significant than 6). */
static bool has16(seq_t x)
{
    if (x == 0)
        return false;
    int prev = -1;               /* digit less significant than the current one */
    while (x) {
        int digit = (int)(x % 9);
        if (prev == 6 && digit == 1)   /* found 1 (more significant) followed by 6 */
            return true;
        prev = digit;
        x /= 9;
    }
    return false;
}

/* Return the nth term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    static seq_t *terms = NULL;   /* cached terms */
    static size_t cap   = 0;      /* allocated capacity of the cache */
    static idx_t  found = 0;      /* number of terms cached */
    static seq_t next_k = 1;      /* next integer to test */

    if (n == 0)          /* OEIS offset is 1, so this case is irrelevant */
        return 0;

    /* Extend the cache until it contains at least n terms. */
    while (found < n) {
        seq_t k  = next_k;
        seq_t k1 = k + 1;

        if (has16(k) && !has16(k1)) {
            /* Grow the cache if necessary. */
            if (found == cap) {
                size_t new_cap = (cap == 0) ? 16 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) abort();       /* allocation failed */
                terms = tmp;
                cap   = new_cap;
            }
            terms[found++] = k;
        }
        next_k = k1;     /* advance to the next integer */
    }
    return terms[n - 1];
}
