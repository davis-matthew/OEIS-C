/*
 * A027185  Triangular array O by rows: O(n,k) = number of partitions of n
 *          into an odd number of parts, the least being k.
 * https://oeis.org/A027185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by flattening the triangular array * O(n,k) =
 *            number of partitions of n into an odd number of parts * whose
 *            smallest part is exactly k. The terms are * listed row by row:
 *            1; 0,1; 1,0,1; 1,0,0,1; ... * This implementation builds all
 *            O(n,k) up to a fixed maximum * number of rows (2...
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Helper macros for table indexing */
#define MAX_ROW 200
#define K_LIMIT (MAX_ROW + 2)
#define IDX(n,k) ((n)*(K_LIMIT)+(k))

/* Static tables for partitions with even and odd number of parts.
   even_table[n][k] = number of partitions of n with parts >= k
   and an even number of parts.
   odd_table[n][k]  = same but an odd number of parts.          */
static seq_t *even_table = NULL;
static seq_t *odd_table  = NULL;
static bool initialized  = false;

/* Build the tables once.  The computation is O(MAX_ROW^2). */
static void build_tables(void)
{
    size_t total = (size_t)(MAX_ROW + 1) * K_LIMIT;
    even_table = (seq_t *)malloc(total * sizeof(seq_t));
    odd_table  = (seq_t *)malloc(total * sizeof(seq_t));
    if (even_table == NULL || odd_table == NULL) {
        /* In the unlikely event of malloc failure, abort the process */
        abort();
    }

    /* Initialise all entries to zero. */
    for (size_t i = 0; i < total; ++i) {
        even_table[i] = (seq_t)0;
        odd_table[i]  = (seq_t)0;
    }

    /* Base case: P(0,k,even) = 1, P(0,k,odd) = 0 for all k. */
    for (idx_t k = 1; k <= MAX_ROW; ++k) {
        even_table[IDX(0, k)] = (seq_t)1;
        /* odd_table[IDX(0,k)] already 0 */
    }

    /* Fill tables for n = 1 .. MAX_ROW */
    for (idx_t n = 1; n <= MAX_ROW; ++n) {
        for (idx_t k = n; k >= 1; --k) {
            /* even = even(n,k+1) + odd(n-k,k) */
            seq_t even_skip = even_table[IDX(n, k+1)];
            seq_t odd_take  = odd_table[IDX(n - k, k)];
            even_table[IDX(n, k)] = even_skip + odd_take;

            /* odd = odd(n,k+1) + even(n-k,k) */
            seq_t odd_skip  = odd_table[IDX(n, k+1)];
            seq_t even_take = even_table[IDX(n - k, k)];
            odd_table[IDX(n, k)] = odd_skip + even_take;
        }
    }

    initialized = true;
}

/* Return the nth term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    if (n < 1) {
        return (seq_t)0;  /* undefined for n < 1, but safe return */
    }

    /* Ensure tables are built */
    if (!initialized) {
        build_tables();
    }

    /* Determine which row contains the nth term.
       The number of terms up to row r is r*(r+1)/2. */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) {
        ++r;
    }

    /* Column index within row r (1-based) */
    idx_t prev_terms = (r - 1) * r / 2;
    idx_t c = n - prev_terms;

    /* O(r,c) = odd(r,c) - odd(r,c+1) */
    seq_t odd_c    = odd_table[IDX(r, c)];
    seq_t odd_c1   = odd_table[IDX(r, c + 1)];
    return odd_c - odd_c1;
}
