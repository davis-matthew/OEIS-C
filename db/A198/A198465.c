/*
 * A198465  Consider triples a<=b<c where (a^2+b^2-c^2)/(c-a-b) = -1, ordered
 *          by a and then b; sequence gives a, b and c values in that order.
 * https://oeis.org/A198465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A198465:  nonn
   Formula: a(n) = the n-th value (a,b,or c) from the list of triples
   (a,b,c) with a ≤ b < c satisfying
          a(a-1) + b(b-1) = c(c-1),
   ordered first by a, then by b.  The sequence outputs the values
   a,b,c of the triples consecutively.
*/

#include <stddef.h>

/* auxiliary struct for a candidate triple (b,c) */
typedef struct { seq_t b; seq_t c; } tc_t;

/* swap two tc_t values (compatible with C) */
static void swap_tc(tc_t *x, tc_t *y)
{
    tc_t t = *x;
    *x = *y;
    *y = t;
}

/* main sequence generator */
seq_t a(idx_t n)
{
    idx_t count = 0;                           /* number of terms produced so far */
    /* iterate over possible first elements a (starting from 3) */
    for (idx_t a_val = 3; ; ++a_val) {
        seq_t A   = (seq_t)a_val;
        seq_t D   = A * (A - (seq_t)1);       /* D = a(a-1) */

        /* temporary storage for all solutions with this a */
        tc_t sols[64];
        idx_t solcnt = 0;

        /* enumerate possible k = c - b (≥1) */
        for (idx_t k_val = 1; ; ++k_val) {
            seq_t K   = (seq_t)k_val;
            seq_t Kk  = K * (K - (seq_t)1);  /* k(k-1) */
            if (Kk >= D) break;             /* no further solutions possible */

            seq_t num = D - Kk;
            seq_t den = (seq_t)2 * K;        /* 2k */

            if (num % den == (seq_t)0) {
                seq_t b_val = num / den;     /* y = b */
                if (b_val >= A) {
                    sols[solcnt].b = b_val;
                    sols[solcnt].c = b_val + K; /* c = b + k */
                    solcnt++;
                }
            }
        }

        /* sort the solutions by b (ascending) – insertion sort for tiny array */
        for (idx_t i = 1; i < solcnt; ++i) {
            for (idx_t j = i; j > 0 && sols[j-1].b > sols[j].b; --j) {
                swap_tc(&sols[j-1], &sols[j]);
            }
        }

        /* output terms a, b, c for each solution */
        for (idx_t i = 0; i < solcnt; ++i) {
            /* output a */
            count++;
            if (count == n) return A;
            /* output b */
            count++;
            if (count == n) return sols[i].b;
            /* output c */
            count++;
            if (count == n) return sols[i].c;
        }

        /* if we have produced enough terms, stop */
        if (count >= n) break;
    }

    /* n is guaranteed to be >=1; the loop should always return before reaching here */
    return (seq_t)0;
}
