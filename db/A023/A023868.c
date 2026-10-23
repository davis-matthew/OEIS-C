/*
 * A023868  a(n) = 1*t(n) + 2*t(n-1) + ... + k*t(n+1-k), where
 *          k=floor((n+1)/2) and t is A023533.
 * https://oeis.org/A023868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=1..floor((n+1)/2)} j * A023533(n-j+1)
 * Range:     u8 n <= 307, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_tetra
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_tetra.h"

static uint8_t *tbool = NULL;   /* 1 if i is tetrahedral, 0 otherwise */
static idx_t tcap = 0;

/* Ensure that tbool[1..n] is computed */
static void prepare_tetra(idx_t n)
{
    if (n <= tcap) return;
    idx_t new_cap = n;
    uint8_t *new_ptr = (uint8_t *)realloc(tbool, (new_cap + 1) * sizeof(uint8_t));
    if (!new_ptr) return; /* allocation failure: keep previous table */
    tbool = new_ptr;
    /* initialise new entries */
    for (idx_t i = tcap + 1; i <= new_cap; ++i) {
        tbool[i] = is_tetra(i) ? 1 : 0;
    }
    tcap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1 – behaviour below 1 is undefined */
    prepare_tetra(n);
    idx_t k = (n + 1) / 2;
    seq_t sum = 0;
    for (idx_t j = 1; j <= k; ++j) {
        idx_t pos = n - j + 1;
        if (tbool[pos])
            sum += (seq_t)j;
    }
    return sum;
}
