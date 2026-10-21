/*
 * A072725  Integers which are exactly the concatenation of the first m
 *          numbers (A007908) divided by their sum (A000217 = m*(m+1)/2).
 * https://oeis.org/A072725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, bref
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A072725 – nth term of the sequence of quotients.
   For each m ≥ 1, let C(m) be the integer obtained by concatenating
   the decimal representations of 1,2,…,m, and let T(m)=m*(m+1)/2
   be the m‑th triangular number.  
   A term occurs when C(m) is divisible by T(m); in that case the
   quotient Q(m)=C(m)/T(m) is part of the sequence.  
   This function searches for such m in increasing order and returns
   the n‑th found quotient.  It memoises already computed terms,
   automatically extending the search when a larger n is requested.  */

#include <stdlib.h>

/* Number of digits of an unsigned number. */
static inline idx_t num_digits(idx_t x)
{
    idx_t d = 0;
    do { d++; x /= 10; } while (x);
    return d;
}

seq_t a(idx_t n)
{
    /* Memoised list of found quotients. */
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;

    /* State of the incremental search. */
    static seq_t   concat = 0;  /* C(m) up to current m */
    static idx_t   cur_m   = 0; /* last m processed */

    /* Extend the list until we have n terms. */
    while (terms_len < (size_t)n) {

        /* Advance to next m. */
        cur_m++;

        /* Update concatenation: C(m) = C(m-1)*10^digits(m) + m */
        idx_t digits = num_digits(cur_m);
        seq_t pow10d = 1;
        for (idx_t i = 0; i < digits; ++i) pow10d *= 10;
        concat = concat * pow10d + cur_m;

        /* Triangular number T(m) = m*(m+1)/2 */
        seq_t sum = (seq_t)cur_m * (cur_m + 1) / 2;

        /* Check divisibility and store quotient if integer. */
        if (concat % sum == 0) {
            seq_t term = concat / sum;
            if (terms_len == terms_cap) {
                terms_cap = terms_cap == 0 ? 4 : terms_cap * 2;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = term;
        }

        /* Safety limit – stop if the search has gone too far.  The
           sequence is conjectured to be finite, so we avoid an
           infinite loop in case of an unexpected behaviour. */
        if (cur_m > 1000000) {
            break;
        }
    }

    /* Return the requested term if it exists, otherwise 0. */
    if (n <= terms_len) {
        return terms[n - 1];
    } else {
        return (seq_t)0;
    }
}
