/*
 * A008975  Triangle, read by rows, formed by reading Pascal's triangle
 *          (A007318) mod 10.
 * https://oeis.org/A008975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, base
 * Formula:   a(n) = C(r, k) mod 10 where n = r*(r-1)/2 + k, 0 ≤ k ≤ r
 * Range:     u8 n <= 9574, u16 n <= 9574, u32 n <= 9574, u64 n <= 9574,
 *            u128 n <= 9574, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A008975.  The sequence
   is the triangle of binomial coefficients modulo 10 read row by row. */
#include <stdlib.h>

static unsigned char *a_cache   = NULL;   /* cache of computed a(n)  */
static size_t         a_cache_sz = 0;      /* number of cached terms   */
static size_t         a_cache_cap = 0;     /* storage allocated for a_cache */

static unsigned char *prev_row   = NULL;   /* last produced row of Pascal mod 10 */
static size_t         prev_row_sz = 0;     /* number of elements in prev_row */

static void push_cache(unsigned char v)
{
    if (a_cache_sz == a_cache_cap) {
        size_t new_cap = a_cache_cap ? a_cache_cap * 2 : 16;
        unsigned char *tmp = (unsigned char *)realloc(a_cache, new_cap);
        a_cache = tmp;
        a_cache_cap = new_cap;
    }
    a_cache[a_cache_sz++] = v;
}

seq_t a(idx_t n)
{
    /* Initialise on first call.  Generate row 0. */
    if (a_cache_sz == 0) {
        prev_row_sz = 1;
        prev_row = (unsigned char *)malloc(sizeof(unsigned char));
        prev_row[0] = 1;
        push_cache(1);            /* a(0) = 1 */
    }

    /* Generate rows until we have at least n+1 cached terms. */
    while (a_cache_sz <= (size_t)n) {
        size_t next_sz = prev_row_sz + 1;
        unsigned char *next_row = (unsigned char *)malloc(next_sz);

        /* Compute the new row from the previous row, keeping values mod 10. */
        for (size_t k = 0; k < next_sz; ++k) {
            unsigned char left  = (k > 0)            ? prev_row[k-1] : 0;
            unsigned char right = (k < prev_row_sz) ? prev_row[k]   : 0;
            next_row[k] = (left + right) % 10;
            push_cache(next_row[k]);               /* store term in global cache */
        }

        /* Prepare for the next iteration.  Free old row. */
        free(prev_row);
        prev_row = next_row;
        prev_row_sz = next_sz;
    }

    return (seq_t)a_cache[(size_t)n];
}
