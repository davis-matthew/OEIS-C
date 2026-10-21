/*
 * A065052  Let R(n) = n-th term of Recamán's sequence A005132; write R(n) =
 *          q*n + r with 0 <= r < n; sequence gives values of r.
 * https://oeis.org/A065052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Recamán(n) mod n, where Recamán(0) = 0 and Recamán(k) =
 *            Recamán(k-1) + k if that value has not been seen before,
 *            otherwise Recamán(k) = Recamán(k-1) - k
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes needed for dynamic allocation */
#include <stdlib.h>

/* Computes a(n) according to the definition of A065052. */
seq_t a(idx_t n)
{
    /* Static storage for computed terms of Recamán’s sequence.  */
    static unsigned long long *rec_terms = NULL;
    static size_t rec_len      = 0;   /* number of terms stored so far   */
    static size_t rec_capacity = 0;   /* allocated capacity               */

    /* --- initialise storage on first call -------------------------------- */
    if (rec_terms == NULL) {
        rec_capacity = 1;
        rec_terms    = (unsigned long long *)malloc(rec_capacity * sizeof(unsigned long long));
        /* Recamán(0) = 0 */
        rec_terms[0] = 0;
        rec_len = 1;
    }

    /* --- extend the sequence up to index n -------------------------------- */
    while (rec_len <= (size_t)n) {
        size_t i      = rec_len;                     /* step size = i   */
        unsigned long long prev = rec_terms[rec_len - 1];
        unsigned long long step = (unsigned long long)i;
        unsigned long long cand_sub, candidate;

        /* Prefer subtraction if legal and the candidate has not appeared before */
        if (prev >= step) {
            cand_sub = prev - step;
            int seen = 0;
            for (size_t j = 0; j < rec_len; ++j) {
                if (rec_terms[j] == cand_sub) { seen = 1; break; }
            }
            if (!seen) {
                candidate = cand_sub;
            } else {
                candidate = prev + step;
            }
        } else {
            candidate = prev + step;
        }

        /* Grow the array if necessary */
        if (rec_len == rec_capacity) {
            rec_capacity = (rec_capacity * 2 > 16) ? rec_capacity * 2 : 16;
            rec_terms = (unsigned long long *)realloc(rec_terms, rec_capacity * sizeof(unsigned long long));
        }
        rec_terms[rec_len++] = candidate;
    }

    /* Recamán(n) now resides at rec_terms[n] */
    unsigned long long n_ll = (unsigned long long)n;
    seq_t r = (seq_t)(rec_terms[n] % n_ll);

    return r;
}
