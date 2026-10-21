/*
 * A055053  Numbers of the form 4^i*(8j+7) or 4^i*(5j+-2).
 * https://oeis.org/A055053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest positive integer n with n = 4^i*(8j+7) or n =
 *            4^i*(5j±2) for some i,j≥0. The function generates the ascending
 *            list of such numbers using a simple sieve-like scan and memoises
 *            results
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Public API as required by the framework */
seq_t a(idx_t n) {
    /* Offset: a(1) is the first term (2). */
    if (n == 0)
        return 0;  /* Undefined for n = 0; return 0 for safety */

    /* Static storage for generated terms */
    static seq_t *terms = NULL; /* array of seq_t values */
    static idx_t count = 0;     /* number of terms currently stored */
    static idx_t max_seen = 0;  /* last number examined during generation */

    /* Ensure capacity (grow exponentially) */
    static size_t cap = 0;
    if (count == 0) {
        cap = 64;
        terms = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!terms)
            return 0;  /* allocation failed: return 0 as error */
    }

    /* Extend generation until we have at least n terms */
    while (count < n) {
        idx_t cur = max_seen + 1;          /* next candidate integer */
        seq_t v = (seq_t)cur;              /* view as seq_t for operations */

        /* Strip off powers of 4 from v */
        while ((v % ((seq_t)4)) == 0)
            v /= ((seq_t)4);

        /* Test the residue conditions */
        bool ok = false;
        if ((v % ((seq_t)8)) == 7)                /* 8j+7 case */
            ok = true;
        else if ((v % ((seq_t)5)) == 2)           /* 5j+2 case */
            ok = true;
        else if ((v % ((seq_t)5)) == 3)           /* 5j-2 (i.e., 5j+3) case */
            ok = true;

        if (ok) {
            /* Store the term, growing array if necessary */
            if (count == cap) {
                cap *= 2;
                terms = (seq_t *)realloc(terms, cap * sizeof(seq_t));
                if (!terms)
                    return 0;  /* allocation failed */
            }
            terms[count++] = (seq_t)cur;
        }
        max_seen = cur;  /* update the maximum examined number */
    }

    /* Return the n-th term (1‑based index) */
    return terms[n - 1];
}
