/*
 * A145312  Coefficients in expansion of E''_4(q), where E_4 is the Eisenstein
 *          series in A004009.
 * https://oeis.org/A145312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 240 * (n+1) * (n+2) * σ₃(n+2) where σ₃(m) is the sum of
 *            the cubes of the divisors of m
 * Range:     u8 none, u16 n <= 1, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* --------------  Memoised σ₃ table  -------------- */
static seq_t *sigma3_tbl = NULL;   /* stored σ₃ values */
static idx_t sigma3_max = 0;       /* highest m currently computed */

/* Pre‑compute σ₃ up to “need”.  Results are stored in sigma3_tbl. */
static void ensure_sigma3(idx_t need)
{
    if (need <= sigma3_max) return;               /* already ready */

    /* Allocate a new table */
    seq_t *new_tbl = (seq_t *)malloc((need + 1) * sizeof(seq_t));
    if (!new_tbl) exit(EXIT_FAILURE);            /* cannot allocate */

    /* initialise to zero */
    for (idx_t i = 0; i <= need; ++i) new_tbl[i] = 0;

    /* classic divisor sieve: for each i add i³ to all multiples of i */
    for (idx_t i = 1; i <= need; ++i) {
        seq_t cube = (seq_t)i * (seq_t)i * (seq_t)i;
        for (idx_t j = i; j <= need; j += i) {
            new_tbl[j] += cube;
        }
    }

    /* replace old table */
    if (sigma3_tbl) free(sigma3_tbl);
    sigma3_tbl = new_tbl;
    sigma3_max = need;
}

/* --------------  Sequence function  -------------- */

seq_t a(idx_t n)
{
    idx_t m = n + 2;                       /* argument of σ₃ */
    ensure_sigma3(m);                      /* make sure σ₃(m) is available */

    seq_t sigma   = sigma3_tbl[m];          /* σ₃(n+2) */
    seq_t result  = (seq_t)240;
    result *= (seq_t)(m - 1);               /* (n+1)  */
    result *= (seq_t)m;                     /* (n+2)  */
    result *= sigma;                        /* times σ₃(n+2) */

    return result;
}
