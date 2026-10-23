/*
 * A102357  "True so far" sequence: floor(a(n)/10) is the number of digits
 *          (a(n) mod 10) within the first n terms; a(n) is the smallest such
 *          number larger than a(n-1).
 * https://oeis.org/A102357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A102357:  
   a(1) = 10;  
   For n > 1,  a(n) is the smallest integer larger than a(n-1)
   for which floor(a(n)/10) equals the total number of occurrences
   of the last digit (a(n) mod 10) among the first n terms. */
#include <stddef.h>

#ifndef MAX_A102357_TERMS
/* The sequence is known to terminate at a(2024).  We allocate enough
   space for all terms that exist. */
#define MAX_A102357_TERMS 2024
#endif

static seq_t terms[MAX_A102357_TERMS + 1] = {0};
static idx_t  digit_counts[10]  = {0};
static idx_t  computed_terms    = 0;

/* Find the smallest term > prev that satisfies the condition.  The
   function starts searching at prev + 1 and increments until a valid
   candidate is found.  It then updates the global digit counts and
   stores the new term.  Because the sequence is finite, the loop
   always terminates for the first 2024 terms. */
static void compute_next_term(void)
{
    seq_t prev = terms[computed_terms - 1];
    seq_t cand = prev + (seq_t)1;

    for (;;) {
        /* Count the occurrences of each digit in cand. */
        idx_t occ[10] = {0};
        seq_t tmp = cand;
        while (tmp > (seq_t)0) {
            int d = (int)(tmp % (seq_t)10);
            occ[d] += (idx_t)1;
            tmp /= (seq_t)10;
        }

        int last_digit = (int)(cand % (seq_t)10);
        idx_t required  = digit_counts[last_digit] + occ[last_digit];
        idx_t floor_div = (idx_t)(cand / (seq_t)10);

        if (floor_div == required) {
            /* cand is the next term.  Update global state. */
            for (int d = 0; d < 10; ++d) {
                digit_counts[d] += occ[d];
                terms[computed_terms] = cand;
            }
            computed_terms++;
            return;
        }
        cand += (seq_t)1;
    }
}

/* Ensure that the array is filled up to at least n terms. */
static void ensure_terms(idx_t n)
{
    if (n <= computed_terms || computed_terms >= MAX_A102357_TERMS)
        return;
    while (computed_terms < n && computed_terms < MAX_A102357_TERMS)
        compute_next_term();
}

/* Public sequence function.  For n > MAX_A102357_TERMS the value of the
   last existing term is returned. */
seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;

    if (n > MAX_A102357_TERMS)
        n = MAX_A102357_TERMS;

    ensure_terms(n);
    return terms[n - 1];
}
