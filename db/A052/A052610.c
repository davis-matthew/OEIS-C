/*
 * A052610  Expansion of e.g.f. 1/(1-x-2*x^3).
 * https://oeis.org/A052610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A052610: Expansion of e.g.f. 1/(1-x-2*x^3)
 * Recurrence:  a(0)=1, a(1)=1, a(2)=2,
 *              a(n+3) = (2*n^3+12*n^2+22*n+12)*a(n) + (n+3)*a(n+2)   (for n ≥ 0)
 *
 * The function memoizes already computed terms so that calls may be in any
 * order.  All arithmetic is performed with the build‑provided type seq_t.
 */

#include <stdlib.h>          /* malloc, realloc, abort */

seq_t a(idx_t n)
{
    /* static table of already computed terms */
    static seq_t *table = NULL;
    static idx_t len   = 0;   /* number of elements stored in table */

    /* allocate or extend the table to hold indices up to n */
    if (n >= len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(table, new_len * sizeof(seq_t));
        if (tmp == NULL) abort();          /* allocation failure */
        table = tmp;
        /* compute any new entries one by one */
        for (idx_t i = len; i < new_len; ++i) {
            if (i == 0)                     table[i] = (seq_t)1;
            else if (i == 1)                 table[i] = (seq_t)1;
            else if (i == 2)                 table[i] = (seq_t)2;
            else {
                /* recurrence with n = i-3 */
                idx_t k          = i - 3;            /* current n in the
                                                        recurrence          */
                seq_t a_k        = table[k];
                seq_t a_k_plus_2 = table[k + 2];

                /* compute the coefficient 2*k^3 + 12*k^2 + 22*k + 12 */
                seq_t coeff = (seq_t)2 * (seq_t)k * (seq_t)k * (seq_t)k
                          + (seq_t)12 * (seq_t)k * (seq_t)k
                          + (seq_t)22 * (seq_t)k
                          + (seq_t)12;

                table[i] = coeff * a_k + ((seq_t)(k + 3)) * a_k_plus_2;
            }
        }
        len = new_len;
    }
    return table[n];
}
