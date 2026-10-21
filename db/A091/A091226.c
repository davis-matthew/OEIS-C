/*
 * A091226  Number of irreducible GF(2)[X]-polynomials in range [0,n].
 * https://oeis.org/A091226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   count of monic irreducible polynomials over GF(2) with numeric
 *            value ≤ n
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   highbit, is_irreducible
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a memoised sieve that checks each integer polynomial at most once. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/highbit.h"
#include "../utils/is_irreducible.h"

/* Memoised table of all irreducible polynomials found so far. */
static seq_t *irred = NULL;
static size_t irred_len   = 0;
static size_t irred_cap   = 0;
static idx_t processed_max = 0;  /* highest n that has been processed */

/* Ensure that every integer in (processed_max, n] has been examined for
   irreducibility and added to the table if appropriate. */
static void ensure_irreducibles_upto(idx_t n) {
    if (n <= processed_max)
        return;

    for (idx_t i = processed_max + 1; i <= n; ++i) {
        seq_t val = (seq_t)i;
        if (is_irreducible(val)) {
            if (irred_len == irred_cap) {
                irred_cap = irred_cap ? (irred_cap << 1) : 16;
                irred = (seq_t *)realloc(irred, irred_cap * sizeof(seq_t));
            }
            irred[irred_len++] = val;
        }
    }
    processed_max = n;
}

/* Count how many stored irreducible values are ≤ the given limit (inclusive). */
static seq_t count_leq(seq_t limit) {
    size_t lo = 0, hi = irred_len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (irred[mid] <= limit)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (seq_t)lo;
}

/* Public sequence function */
seq_t a(idx_t n) {
    ensure_irreducibles_upto(n);
    return count_leq((seq_t)n);
}
