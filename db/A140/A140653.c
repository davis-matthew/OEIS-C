/*
 * A140653  A triangular sequence of cyclotomic polynomial doubling: For
 *          cyclotomic polynomial:c(x,n) p(x,n)=c(x,Prime[n])*c(x,2*Prime[n]).
 * https://oeis.org/A140653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, uned
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A140653
 *
 * A(n)=1 if the index n lies in a row of the triangular array that starts
 * with a 1 and alternates 1,0,1,0,…; all positions of the first row are 1.
 * The first row has length 4, each subsequent row r (r≥2) has length
 * 2·Prime[r]−1, where Prime[1]=2, Prime[2]=3, ….
 * a(n)=1 for odd positions in r≥2, otherwise a(n)=0.
 *
 * The implementation builds primes and row lengths lazily and keeps a
 * cumulative sum of row lengths to locate the row containing a given index.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/isPrimeTrial.h"

/* static data for memoisation */
static size_t          primes_capacity = 0;
static uint64_t       *primes_array     = NULL;   /* primes[0] = 2, primes[1] = 3, … */
static idx_t          *cumulative_len   = NULL;   /* cumulative_len[i] = total length up to row i+1 */
static size_t          prime_count      = 0;       /* number of rows already generated */

/* ------------------------------------------------------------------ */
/* enlarge the prime and cumulative tables to hold at least new_cap   */
/* ------------------------------------------------------------------ */
static void grow_tables(size_t new_cap)
{
    uint64_t *new_primes;
    idx_t    *new_cum;

    new_primes = (uint64_t *)realloc(primes_array, new_cap * sizeof *new_primes);
    new_cum    = (idx_t    *)realloc(cumulative_len, new_cap * sizeof *new_cum);
    if (!new_primes || !new_cum)
        abort();                       /* memory allocation failure */

    primes_array     = new_primes;
    cumulative_len   = new_cum;
    primes_capacity  = new_cap;
}

/* ------------------------------------------------------------------ */
/* ensure that cumulative_len[prime_count-1] ≥ target                */
/* ------------------------------------------------------------------ */
static void ensure_cumulative(idx_t target)
{
    while (prime_count == 0 || cumulative_len[prime_count - 1] < target) {
        /* grow arrays if necessary */
        if (prime_count >= primes_capacity)
            grow_tables(primes_capacity ? primes_capacity * 2U : 8U);

        /* find next prime */
        uint64_t p;
        if (prime_count == 0) {
            p = 2ULL;
        } else {
            p = primes_array[prime_count - 1] + 1ULL;
            while (!isPrimeTrial((seq_t)p))
                ++p;
        }
        primes_array[prime_count] = p;

        /* length of this row */
        idx_t row_len;
        if (prime_count == 0)            /* row 1 */
            row_len = 4U;
        else                             /* rows r≥2 */
            row_len = (idx_t)(2U * p - 1U);

        /* cumulative length */
        if (prime_count == 0)
            cumulative_len[prime_count] = row_len;
        else
            cumulative_len[prime_count] = cumulative_len[prime_count - 1] + row_len;

        ++prime_count;
    }
}

/* ------------------------------------------------------------------ */
/* a(n) implementation                                                  */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for offset, return 0 */
        return (seq_t)0;

    ensure_cumulative(n);

    /* locate the row containing n */
    size_t row = 0;
    while (row < prime_count && cumulative_len[row] < n)
        ++row;

    /* position within the row, 1‑based */
    idx_t pos_in_row = n;
    if (row > 0)
        pos_in_row -= cumulative_len[row - 1];

    /* first row: all 1's */
    if (row == 0)
        return (seq_t)1;

    /* rows r≥2 alternate 1,0 starting with 1 → odd positions are 1 */
    return ((pos_in_row & 1U) == 1U) ? (seq_t)1 : (seq_t)0;
}
