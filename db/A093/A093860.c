/*
 * A093860  Begin with the n-th composite number (A002808(n)) and go on adding
 *          successive composite numbers; a(n) is the first prime that arises
 *          as the sum.
 * https://oeis.org/A093860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_prime_big, is_prime_small, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A093860:  a(n) is the first prime obtained as the sum of successive
   composite numbers starting with the n‑th composite number.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"

/* Global storage for composite numbers, 1‑based index. */
static seq_t *composites = NULL;   /* array of composites */
static size_t comp_cap   = 0;      /* allocated capacity */
static size_t comp_size  = 0;      /* number of composites stored */
static idx_t  next_test  = 4;      /* next integer to test for compositeness */

/* Ensure composites[0 .. idx-1] are filled (i.e., first idx composites). */
static void ensure_composite_index(idx_t idx)
{
    while (comp_size < idx) {
        if (next_test >= 4 && !is_prime_small((uint64_t)next_test)) {
            if (comp_size == comp_cap) {
                size_t new_cap = comp_cap ? comp_cap * 2 : 16;
                seq_t *tmp = seq_realloc(composites, comp_cap, new_cap);
                if (!tmp) abort();  /* unrecoverable allocation failure */
                composites = tmp;
                comp_cap = new_cap;
            }
            composites[comp_size++] = (seq_t)next_test;
        }
        ++next_test;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1; behaviour below is unused */

    /* Obtain the n‑th composite number. */
    ensure_composite_index(n);
    seq_t sum = composites[n - 1];

    /* Accumulate successive composites until a prime sum is found. */
    idx_t j = n;                     /* next composite index to add (1‑based) */
    while (!is_prime_big(sum)) {
        ++j;
        ensure_composite_index(j);
        sum += composites[j - 1];
    }
    return sum;
}
