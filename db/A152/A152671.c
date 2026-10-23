/*
 * A152671  Even Catalan numbers divided by 2.
 * https://oeis.org/A152671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A152670(n) / 2 (the n‑th even Catalan number divided by
 *            two)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 13, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


/* Returns the n-th term of OEIS A152671.
   The function recomputes the sequence from scratch each call,
   which is efficient enough for n up to 10 000. */
seq_t a(idx_t n)
{
    if (n < 1) abort();                /* offset is 1, reject n = 0, etc. */

    /* Catalan recurrence: C_{k+1} = C_k * 2*(2k+1)/(k+2) */
    seq_t cat_val = (seq_t)1;          /* C_0 */
    idx_t  k      = 0;                 /* current index of cat_val */
    idx_t  even_cnt = 0;               /* how many even Catalan numbers seen */

    while (even_cnt < n)
    {
        /* Compute next Catalan number C_{k+1} */
        seq_t factor = (seq_t)(2ULL * (2ULL * k + 1ULL)); /* 2*(2k+1) */
        cat_val = (cat_val * factor) / (seq_t)(k + 2);    /* C_{k+1} */
        ++k;

        /* If C_k is even, count it. */
        if ((cat_val & ((seq_t)1)) == 0)
            ++even_cnt;
    }

    /* cat_val now holds the n-th even Catalan number C_k.
       Return it divided by two. */
    return cat_val / (seq_t)2;
}
