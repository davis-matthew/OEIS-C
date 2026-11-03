/*
 * A069699  Triangular numbers with internal digits 7.
 * https://oeis.org/A069699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = T_k = k(k+1)/2 with the property that all internal
 *            decimal digits of the number are 7. In other words, a(n) is the
 *            n‑th triangular number whose decimal representation has only the
 *            first and last digits possibly different from 7
 * Range:     u8 n <= 15, u16 n <= 21, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Validated: full, pass, 21 terms through n = 21 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is finite; we generate all qualifying triangular
   numbers once, store them in a static array, and answer queries
   from that array. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Check whether the internal decimal digits of a number are all '7'.
   Numbers with 0, 1 or 2 digits satisfy the condition vacuously. */
static bool internal_sevens(seq_t val)
{
    /* For very small numbers the loop is trivial. */
    if (val < (seq_t)10) return true;

    int digits[32];
    int len = 0;
    seq_t tmp = val;
    while (tmp > (seq_t)0) {
        digits[len++] = (int)(tmp % (seq_t)10);
        tmp = tmp / (seq_t)10;
        if (len >= 32) break;   /* safety for extremely large numbers */
    }
    if (len <= 2) return true;  /* no internal digits to check */

    /* digits[0] is least significant; digits[len-1] is most significant. */
    for (int i = 1; i < len - 1; ++i) {
        if (digits[len - 1 - i] != 7) return false;
    }
    return true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    static seq_t *terms   = NULL;   /* array of qualifying triangular numbers */
    static size_t len     = 0;      /* number of terms computed so far */
    static bool built     = false;  /* whether the list has been generated */

    /* Build the list lazily on the first call. */
    if (!built) {
        size_t cap = 64;
        terms = mem_new(seq_t, cap);
        if (!terms) return (seq_t)0;  /* allocation failure – return 0 */

        /* Generate triangular numbers T_k = k(k+1)/2 for k = 0,1,2,...,
           up to a sufficiently large bound to capture all occurrences.  */
        const idx_t MAX_K = 20000;  /* large enough for the finite sequence */
        for (idx_t k = 0; k <= MAX_K; ++k) {
            seq_t K = (seq_t)k;
            seq_t T = K * (K + (seq_t)1) / (seq_t)2;   /* exact triangular number */

            if (internal_sevens(T)) {
                /* Grow array if necessary. */
                if (len == cap) {
                    cap *= 2;
                    seq_t *newp = mem_resize(seq_t, terms, cap);
                    if (!newp) { mem_free(terms); return (seq_t)0; }
                    terms = newp;
                }
                terms[len++] = T;
            }
        }
        built = true;
    }

    size_t idx = (size_t)n;
    if (idx == 0 || idx > len) return (seq_t)0;
    return terms[idx - 1];
}
