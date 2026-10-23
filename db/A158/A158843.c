/*
 * A158843  G.f.: A(x) = exp( Sum_{n>=1} A001333(n)^n * 2^n*x^n/n ).
 * https://oeis.org/A158843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = exp( Σ_{k≥1} A001333(k)^k * 2^k * x^k / k )
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   pow2, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The recurrence derived from A(x) = exp(F(x)) with F(x) = Σ g_k x^k gives
   a(0) = 1 and for n≥1:
   a(n) = (1/n) * Σ_{k=1}^n (A001333(k))^k * 2^k * a(n−k)   */
/* A001333(k) = Pell(k+1) – Pell(k) where Pell is the Pell sequence
   defined by Pell(0)=0, Pell(1)=1, Pell(n)=2·Pell(n−1)+Pell(n−2). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow2.h"    /* seq_t pow2(idx_t exp) */
#include "../utils/pow_seq.h" /* seq_t pow_seq(seq_t a, int k) */

/* --------------------------------------------------------------------- */
/*  Dynamic storage for Pell numbers (Pell[0] ... Pell[max_index])       */
static seq_t *pell_cache = NULL;   /* array of Pell numbers */
static size_t pell_cap   = 0;      /* allocated size */
static idx_t  pell_max   = 0;      /* highest index computed */

/* Initialise Pell cache if empty */
static void init_pell(void)
{
    if (pell_cache) return;
    pell_cache = (seq_t *)malloc(2 * sizeof(seq_t));
    if (!pell_cache) exit(1);
    pell_cache[0] = 0;
    pell_cache[1] = 1;
    pell_cap   = 2;
    pell_max   = 1;
}

/* Ensure that Pell numbers up to index `need` are available */
static void ensure_pell(idx_t need)
{
    if (!pell_cache) init_pell();
    if (need <= pell_max) return;

    size_t new_cap = (size_t)(need + 1);
    seq_t *new_ptr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_ptr) exit(1);

    /* copy existing values */
    for (idx_t i = 0; i <= pell_max; ++i) new_ptr[i] = pell_cache[i];

    /* compute missing Pell terms */
    for (idx_t i = pell_max + 1; i <= need; ++i) {
        new_ptr[i] = 2 * new_ptr[i-1] + new_ptr[i-2];
    }

    free(pell_cache);
    pell_cache = new_ptr;
    pell_cap   = new_cap;
    pell_max   = need;
}

/* --------------------------------------------------------------------- */
/*  Dynamic storage for sequence a(n)                                     */
static seq_t *a_cache = NULL;     /* array for a(n) values */
static size_t a_cap   = 0;        /* allocated size */
static idx_t  a_max   = 0;        /* highest n computed */

/* Initialise an array with a(0)=1 if it does not exist yet */
static void init_a_cache(void)
{
    if (a_cache) return;
    a_cache = (seq_t *)malloc(sizeof(seq_t));
    if (!a_cache) exit(1);
    a_cache[0] = 1;   /* a(0) = 1 */
    a_cap   = 1;
    a_max   = 0;
}

/* Expand a_cache to hold indices up to `need` (inclusive) */
static void ensure_a_cache(idx_t need)
{
    if (!a_cache) init_a_cache();
    if (need < a_cap) return;

    size_t new_cap = (size_t)(need + 1);
    seq_t *new_ptr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_ptr) exit(1);

    for (size_t i = 0; i < a_cap; ++i) new_ptr[i] = a_cache[i];
    /* remaining entries will be filled later */
    free(a_cache);
    a_cache = new_ptr;
    a_cap   = new_cap;
}

/* --------------------------------------------------------------------- */
/*  Public sequence function                                              */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    if (!a_cache) init_a_cache();
    if (n <= a_max) return a_cache[n];

    ensure_a_cache(n);

    for (idx_t cur = a_max + 1; cur <= n; ++cur) {
        /* Need Pell up to cur+1 for differences A001333(k) */
        ensure_pell(cur + 1);

        seq_t sum = 0;
        for (idx_t k = 1; k <= cur; ++k) {
            seq_t diff   = pell_cache[k+1] - pell_cache[k];          /* A001333(k) */
            seq_t powd   = pow_seq(diff, (int)k);                    /* (A001333(k))^k */
            seq_t term   = powd * pow2((idx_t)k) * a_cache[cur - k];
            sum += term;
        }
        a_cache[cur] = sum / (seq_t)cur;
    }
    a_max = n;
    return a_cache[n];
}
