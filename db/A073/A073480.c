/*
 * A073480  Triangle T(n,k) read by rows, where e.g.f. for T(n,k) is
 *          exp(x*y)*log(1+x)/(1-x).
 * https://oeis.org/A073480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = binomial(r, k) * A024167(r‑k) * where r is the row
 *            containing index n (1‑based) and * k = n – ((r‑1)*r/2 + 1) is
 *            the 0‑based column index. * A024167(m) is defined by *
 *            A024167(1) = 1 * A024167(m) = m*A024167(m‑1) ± (m‑1)! (+ for odd
 *            m, – for even m) * * For m = 0 we set A024167(0) = 0 and...
 * Range:     u8 n <= 15, u16 n <= 36, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binom.h"

static idx_t table_size = 0;          /* number of entries stored (0..size-1) */
static seq_t *fact_table = NULL;     /* factorials 0! … (size-1)!          */
static seq_t *a024167 = NULL;        /* A024167 values for 0 … (size-1)    */

/* Create or extend the tables so that indices up to `need` exist. */
static void
ensure_tables(idx_t need)
{
    if (need < table_size)
        return;

    idx_t new_size = need + 1;                  /* inclusive of `need` */

    fact_table = (seq_t *)realloc(fact_table, new_size * sizeof(seq_t));
    a024167   = (seq_t *)realloc(a024167,   new_size * sizeof(seq_t));

    if (table_size == 0) {                      /* first allocation   */
        fact_table[0] = (seq_t)1;                /* 0! = 1             */
        a024167[0]   = (seq_t)0;                /* A024167(0) = 0     */
        table_size   = 1;
    }

    for (idx_t i = table_size; i <= need; ++i) {
        seq_t prev_fact = fact_table[i - 1];
        fact_table[i] = prev_fact * (seq_t)i;   /* i! = (i‑1)! * i    */

        if (i % 2 == 0)                        /* even i */
            a024167[i] = (seq_t)i * a024167[i - 1] - prev_fact;
        else                                   /* odd  i */
            a024167[i] = (seq_t)i * a024167[i - 1] + prev_fact;
    }
    table_size = new_size;
}

/* Find the row number r (1‑based) that contains index n. */
static idx_t
row_of(idx_t n)
{
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;
    return r;
}

/* Public function: returns a(n) for n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                    /* not used for the sequence */

    idx_t r      = row_of(n);                               /* 1‑based row   */
    idx_t start  = ((r - 1) * r) / 2 + 1;                    /* first index   */
    idx_t k      = n - start;                               /* 0‑based column */

    idx_t left = r - k;                                     /* index for A024167 */

    ensure_tables((r > left) ? r : left);

    seq_t bin = binom(r, k);
    seq_t val = a024167[left];

    return bin * val;
}
