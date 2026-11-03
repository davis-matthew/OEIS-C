/*
 * A044745  Numbers n such that string 3,2 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth integer (n ≥ 1) whose decimal representation contains
   the substring "32" but whose successor does NOT contain "32".  The
   sequence is indexed with a(1) = 32. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Helper: test whether x contains the digit pattern 3,2 (more‑significant
   3 followed by 2) in its base‑10 representation. */
static bool has_sub_32(seq_t x)
{
    static const seq_t TEN = (seq_t)10;
    seq_t temp = x;
    int prev = -1;                         /* previous (lower) digit */
    while (temp > (seq_t)0) {
        int d = (int)(temp % TEN);         /* current least‑significant digit */
        temp /= TEN;
        if (prev == 2 && d == 3)           /* found "3" followed by "2" */
            return true;
        prev = d;
    }
    return false;
}

/* Simple cache for generated terms.  It is expanded on demand. */
static seq_t *CACHE   = NULL;   /* array of found terms            */
static size_t  SIZE    = 0;     /* number of terms stored          */
static size_t  CAP     = 0;     /* allocated capacity              */
static seq_t  LASTCHK  = (seq_t)31;   /* last integer tested (start 32) */

/* Ensure that at least n terms are stored in CACHE.  This function
   can be called repeatedly with arbitrary n. */
static void ensure_cache(idx_t n)
{
    while (SIZE < n) {
        /* Find the next integer that satisfies the definition. */
        seq_t cand = ++LASTCHK;
        for (;;) {
            if (has_sub_32(cand) && !has_sub_32(cand + (seq_t)1)) {
                /* Store cand.  Grow the array if necessary. */
                if (SIZE == CAP) {
                    CAP = CAP ? CAP * 2 : 16;
                    CACHE = mem_resize(seq_t, CACHE, CAP);
                }
                CACHE[SIZE++] = cand;
                break;                     /* proceed to next term */
            }
            ++cand;
            ++LASTCHK;
        }
    }
}

/* Return the n-th term of the sequence.  The function is safe for any
   n ≥ 1 and works for repeated calls with arbitrary ordering of n. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)          /* the sequence is 1‑based; 0 is undefined */
        return (seq_t)0;
    ensure_cache(n);
    return CACHE[n - 1];        /* cache is 0‑indexed */
}
