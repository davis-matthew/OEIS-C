/*
 * A066847  Integers of the form m! + n!, m and n = positive integers.
 * https://oeis.org/A066847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 77, u64 n <= 209,
 *            u128 n <= 592, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A066847: integers of the form m! + n! (m,n ≥ 1).  
   The function generates the terms in increasing order, removing
   duplicates, and caches the whole list so that successive calls are
   efficient. */

#include <stdlib.h>
#include <stddef.h>

/* ------------------------------------------------------------------ */
/* Dynamically enlarge an array of `seq_t` if necessary.              */
static void ensure_cap(size_t *cap, size_t need, seq_t **arr)
{
    if (*cap < need) {
        size_t newcap = (*cap == 0) ? 16 : *cap * 2;
        while (newcap < need) newcap <<= 1;
        *arr = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
        *cap = newcap;
    }
}

/* ------------------------------------------------------------------ */
/* Comparator for qsort on `seq_t`.                                    */
static int cmp_seq_t(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* ------------------------------------------------------------------ */
/* Main sequence function.                                             */
seq_t a(idx_t n)
{
    /* Static cache of terms and factorials. */
    static seq_t *terms = NULL;     /* sorted unique sums    */
    static size_t terms_count = 0;  /* number of stored terms*/
    static size_t terms_cap   = 0;  /* allocated capacity    */

    static seq_t *fact   = NULL;     /* factorials; fact[i] = i! */
    static size_t fact_count = 0;    /* highest factorial index used */
    static size_t fact_cap   = 0;    /* allocated capacity of fact */

    /* ------------------------------------------------------------------ */
    /* Initialise on first call                                         */
    if (!terms) {
        ensure_cap(&terms_cap, 16, &terms);
        ensure_cap(&fact_cap,   4,  &fact);

        /* 1! = 1  (index 1).                                       */
        fact[0] = (seq_t)0;     /* unused slot for 0! */
        fact[1] = (seq_t)1;
        fact_count = 1;

        /* First term: 1! + 1! = 2.                                 */
        terms[0] = (seq_t)2;
        terms_count = 1;
    }

    /* ------------------------------------------------------------------ */
    /* Generate new terms until we have at least (n+1) of them. */
    while (terms_count <= (size_t)n) {
        /* ---- Compute the next factorial (k! with k = fact_count+1). */
        size_t k = fact_count + 1;
        ensure_cap(&fact_cap, k + 1, &fact);
        seq_t nextFact = fact[fact_count] * (seq_t)k;   /* (k-1)! * k  */
        fact[k] = nextFact;
        fact_count = k;

        /* ---- Produce all sums involving the new factorial. */
        for (size_t i = 1; i <= fact_count; ++i) {
            seq_t sum = nextFact + fact[i];
            ensure_cap(&terms_cap, terms_count + 1, &terms);
            terms[terms_count++] = sum;
        }

        /* ---- Sort the list and remove duplicates. */
        qsort(terms, terms_count, sizeof(seq_t), cmp_seq_t);
        size_t unique = 0;
        for (size_t i = 0; i < terms_count; ++i) {
            if (unique == 0 || terms[i] != terms[unique - 1]) {
                terms[unique++] = terms[i];
            }
        }
        terms_count = unique;
    }

    /* ------------------------------------------------------------------ */
    return terms[(size_t)n];
}
