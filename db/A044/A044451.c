/*
 * A044451  Numbers n such that string 2,1 occurs in the base 4 representation
 *          of n but not of n+1.
 * https://oeis.org/A044451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer that contains the substring 21
 *            in its base‑4 representation and whose successor does not
 *            contain 21 in its base‑4 representation
 * Range:     u8 n <= 20, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Test whether a base‑4 representation of x contains the substring "21" */
static bool has21(seq_t x)
{
    bool prev_one = false;
    while (x > 0) {
        seq_t d = x % 4;            /* least significant digit */
        if (prev_one && d == 2)     /* found "21" (two followed by one) */
            return true;
        prev_one = (d == 1);       /* remember if current digit is 1 */
        x /= 4;
    }
    return false;
}

/* Return the n‑th number satisfying the above property (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                   /* undefined for index 0 */

    static seq_t *terms = NULL;     /* cached terms */
    static idx_t terms_size = 0;    /* number of cached terms */
    static idx_t terms_cap  = 0;    /* allocated capacity */
    static seq_t last_checked = 0;  /* last number examined */

    /* Grow cache until it contains at least n terms */
    while (terms_size < n) {
        seq_t cand = last_checked + 1;

        /* Scan successive integers until one satisfies the property */
        for ( ; ; ++cand) {
            if (has21(cand) && !has21(cand + 1)) {
                /* Ensure capacity for the new term */
                if (terms_size >= terms_cap) {
                    idx_t new_cap = terms_cap ? terms_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(terms,
                        new_cap * sizeof(seq_t));
                    if (!tmp) return 0; /* allocation failed */
                    terms = tmp;
                    terms_cap = new_cap;
                }
                terms[terms_size++] = cand;
                last_checked = cand;
                break;
            }
        }
    }

    return terms[n - 1];
}
