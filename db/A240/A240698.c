/*
 * A240698  Partial sums of divisors of n, cf. A027750.
 * https://oeis.org/A240698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 527, u16 n <= 7069, u32 n <= 7069, u64 n <= 7069,
 *            u128 n <= 7069, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A240698
 * a(n) is the n‑th element of the flattened triangle whose i‑th row is
 * the partial sums of the divisors of i.
 *
 * The implementation builds rows on demand and keeps a cumulative count
 * array cumD[0..max_row] with cumD[i] = total number of elements in rows
 * 1..i.  For a given index n, the required row r and the position k within
 * that row are found by a linear search on cumD.  The row is fully
 * generated: its divisors are listed, sorted, and their prefix sums
 * computed.  The k‑th partial sum is returned as a seq_t value.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ----------------------------------------------------------------------
 *  Smallest prime factor sieve
 * ---------------------------------------------------------------------- */
static idx_t *spf     = NULL;  /* spf[i] = smallest prime factor of i */
static idx_t  spf_cap = 0;      /* current capacity of spf array */

/* Recomputes spf[0..limit] and resizes the array */
static void grow_spf_to(idx_t limit)
{
    if (limit <= spf_cap) return;

    idx_t *new_spf = (idx_t*)realloc(spf, (limit + 1) * sizeof(idx_t));
    if (!new_spf) abort();          /* sanity check */
    spf = new_spf;
    spf_cap = limit;

    /* initialise to 0 */
    for (idx_t i = 0; i <= limit; ++i) spf[i] = 0;

    /* sieve of Eratosthenes */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (spf[i] == 0) {          /* i is prime */
            spf[i] = i;
            for (idx_t j = i * i; j <= limit; j += i)
                if (spf[j] == 0) spf[j] = i;
        }
    }
    for (idx_t i = 2; i <= limit; ++i)
        if (spf[i] == 0) spf[i] = i; /* remaining primes */
}

/* ----------------------------------------------------------------------
 *  Divisor counting using the spf table
 * ---------------------------------------------------------------------- */
static idx_t divisor_count(idx_t n)
{
    if (n == 1) return 1;
    idx_t result = 1;
    while (n > 1) {
        idx_t p   = spf[n];
        idx_t exp = 0;
        while (n % p == 0) {
            n /= p;
            ++exp;
        }
        result *= (exp + 1);
    }
    return result;
}

/* ----------------------------------------------------------------------
 *  Row construction helpers
 * ---------------------------------------------------------------------- */
static idx_t *cumD   = NULL;  /* cumulative element counts */
static idx_t *numDiv = NULL;  /* divisor counts per row */
static idx_t  cum_cap = 0;    /* capacity of the above arrays */
static idx_t  max_row  = 0;   /* highest row constructed */

/* Ensures that rows up to one that contains at least 'n' elements are built */
static void ensure_rows_to(idx_t n)
{
    if (cumD == NULL) {         /* first call – initialise */
        cum_cap = 2;
        cumD   = (idx_t*)malloc(cum_cap * sizeof(idx_t));
        numDiv = (idx_t*)malloc(cum_cap * sizeof(idx_t));
        if (!cumD || !numDiv) abort();
        cumD[0]   = 0;
        numDiv[0] = 0;
        max_row = 0;
    }

    while (cumD[max_row] < n) {
        idx_t new_row = max_row + 1;

        /* grow the cumulative arrays if necessary */
        if (new_row >= cum_cap) {
            idx_t new_cap = cum_cap * 2;
            while (new_row >= new_cap) new_cap *= 2;
            idx_t *tmp;

            tmp = (idx_t*)realloc(cumD,   new_cap * sizeof(idx_t));
            if (!tmp) abort(); cumD   = tmp;

            tmp = (idx_t*)realloc(numDiv, new_cap * sizeof(idx_t));
            if (!tmp) abort(); numDiv = tmp;

            cum_cap = new_cap;
        }

        /* ensure spf table covers up to new_row */
        if (new_row > spf_cap) grow_spf_to(new_row);

        idx_t d = divisor_count(new_row);
        numDiv[new_row] = d;
        cumD[new_row]   = cumD[max_row] + d;
        max_row = new_row;
    }
}

/* ----------------------------------------------------------------------
 *  Comparator for qsort on idx_t
 * ---------------------------------------------------------------------- */
static int cmp_idx_t(const void *a, const void *b)
{
    idx_t va = *(const idx_t*)a;
    idx_t vb = *(const idx_t*)b;
    return (va > vb) - (va < vb);
}

/* ----------------------------------------------------------------------
 *  Main sequence function
 * ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* sequence is 1‑indexed */

    /* Build rows sufficient to cover the requested index n */
    ensure_rows_to(n);

    /* Locate the row r that contains the n‑th element */
    idx_t r = 1;
    while (cumD[r] < n) ++r;              /* cumD[0] = 0 */

    idx_t prevCum = cumD[r - 1];
    idx_t pos     = n - prevCum;          /* 1‑based position in row r */

    /* Generate the divisor list of r */
    idx_t temp = r;
    idx_t primes[64], exps[64], pf_cnt = 0;
    while (temp > 1) {
        idx_t p   = spf[temp];
        idx_t exp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }
        primes[pf_cnt] = p;
        exps[pf_cnt]   = exp;
        ++pf_cnt;
    }

    /* Build all divisors */
    idx_t d      = numDiv[r];
    size_t dsize = (size_t)d;
    idx_t *divisors = (idx_t*)malloc(dsize * sizeof(idx_t));
    if (!divisors) abort();
    size_t sz = 1;
    divisors[0] = 1;

    for (size_t i = 0; i < pf_cnt; ++i) {
        idx_t prime = primes[i];
        idx_t exp   = exps[i];
        size_t cur_sz = sz;
        idx_t power = 1;
        for (idx_t k = 1; k <= exp; ++k) {
            power *= prime;
            for (size_t j = 0; j < cur_sz; ++j)
                divisors[sz++] = divisors[j] * power;
        }
    }

    /* Ensure the divisors are sorted */
    qsort(divisors, dsize, sizeof(idx_t), cmp_idx_t);

    /* Compute the required partial sum (prefix up to pos) */
    seq_t acc = 0;
    for (idx_t i = 0; i < pos; ++i)
        acc += (seq_t)divisors[i];

    free(divisors);
    return acc;
}
