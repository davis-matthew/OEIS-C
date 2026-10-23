/*
 * A127755  Inverse of number triangle A(n,k) = 1/n! if k <= n <= 2k, 0
 *          otherwise.
 * https://oeis.org/A127755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 26, i16 n <= 43, i32 n <= 63, i64 n <= 63, i128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Inverse triangle A127755
 * Recurrence:  for n>0 and k<n
 *   a[n,k] = -∑_{j=max(k,ceil(n/2))}^{n-1} a[j,k]
 *   a[n,n] = n!
 * a(n) is the flattened row‑major form of the triangle:
 *   a(0)=1; a(1)=0,a(2)=1; a(3)=0,a(4)=-1,a(5)=2; ...
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Helper: start index of row r in the flattened array */
static inline size_t start_of_row(idx_t r)
{
    /* r*(r+1)/2 fits into a size_t for the intended n */
    return (size_t)(r * (r + 1) / 2);
}

/* Cached data */
static seq_t *values      = NULL;  /* flattened triangle */
static size_t values_len  = 0;     /* current number of stored elements */
static size_t values_cap  = 0;     /* capacity of values array */

static seq_t *fact        = NULL;  /* factorials */
static size_t fact_len    = 0;     /* number of stored factorials (fact_len == r+1 once r rows computed) */
static size_t fact_cap    = 0;     /* capacity of factorial array */

static idx_t  rows        = 0;     /* number of rows already computed */

/* Store a value in the flattened array, growing the storage if necessary */
static void push_value(seq_t x)
{
    if (values_len == values_cap) {
        size_t new_cap = values_cap ? values_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(values, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failure – return zero as a safe default */
            return;
        }
        values = tmp;
        values_cap = new_cap;
    }
    values[values_len++] = x;
}

/* Compute factorials up to r, allocating more space if needed */
static void ensure_factorial(idx_t r)
{
    if (fact_len <= r) {
        /* Ensure enough capacity */
        size_t new_cap = fact_cap ? fact_cap * 2 : 4;
        if (new_cap <= (size_t)r + 1)
            new_cap = (size_t)r + 1;
        seq_t *tmp = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
        if (!tmp) {
            return;
        }
        fact = tmp;
        fact_cap = new_cap;
        /* Compute missing factorials */
        for (size_t i = fact_len; i <= (size_t)r; ++i) {
            if (i == 0)
                fact[0] = (seq_t)1;
            else
                fact[i] = fact[i - 1] * (seq_t)i;
        }
        fact_len = (size_t)r + 1;
    }
}

/* Main API: returns a(n) */
seq_t a(idx_t n)
{
    /* Compute all needed rows up to n */
    while (rows <= n) {
        idx_t r = rows;          /* current row index to compute */

        /* Ensure factorial for this row is available */
        ensure_factorial(r);

        /* Compute all entries of row r */
        for (idx_t k = 0; k < r; ++k) { /* off‑diagonal elements */
            /* lower bound of the summation */
            idx_t lower = (r + 1) / 2;   /* ceil(r/2) */
            idx_t j0 = k < lower ? lower : k;
            seq_t sum = (seq_t)0;
            for (idx_t j = j0; j < r; ++j) {
                size_t idx = start_of_row(j) + k;
                sum += values[idx];
            }
            push_value(-sum);
        }

        /* Diagonal element */
        seq_t diag = fact[r];
        push_value(diag);

        ++rows;
    }

    /* Return the cached value at position n */
    if (n >= values_len) {
        /* Should never happen, but return 0 safely. */
        return (seq_t)0;
    }
    return values[(size_t)n];
}
