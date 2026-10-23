/*
 * A131798  a(n) = the maximum value from among
 *          (d(n+1),d(n+2),d(n+3),...,d(2n)), where d(m) is the number of
 *          positive divisors of m.
 * https://oeis.org/A131798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = max{ d(m) | n < m ≤ 2n } using a divisor‑count sieve up to 2n. */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static uint32_t *div_counts = NULL;   /* divisor counts for indices 0 … L‑1 */
static size_t   div_limit  = 0;       /* current array size (max index + 1) */
static idx_t    div_n_max  = 0;       /* highest n for which the sieve was built */

/* Rebuild the divisor‑count sieve for all numbers up to 2·max_n. */
static void rebuild_sieve(idx_t max_n)
{
    size_t new_limit = (size_t)((seq_t)max_n * 2 + 1);   /* indices 0 … 2·max_n */

    /* Allocate or resize the array. */
    uint32_t *tmp = (uint32_t*)realloc(div_counts, new_limit * sizeof(uint32_t));
    if (tmp == NULL) {
        exit(EXIT_FAILURE);   /* allocation failure */
    }
    div_counts = tmp;

    /* Zero the whole array up to new_limit. */
    for (size_t i = 0; i < new_limit; ++i) {
        div_counts[i] = 0;
    }

    /* Sieve divisor counts. */
    for (size_t i = 1; i < new_limit; ++i) {
        for (size_t j = i; j < new_limit; j += i) {
            ++div_counts[j];
        }
    }

    div_limit = new_limit;
    div_n_max = max_n;
}

seq_t a(idx_t n)
{
    if (n < 1) {
        return (seq_t)0;    /* undefined for n < 1 */
    }

    /* Ensure the sieve covers 2·n. */
    if (n > div_n_max) {
        rebuild_sieve(n);
    }

    seq_t max_d = (seq_t)0;
    idx_t upper = (idx_t)((seq_t)n * 2);
    for (idx_t m = n + 1; m <= upper; ++m) {
        seq_t d = (seq_t)div_counts[(size_t)m];
        if (d > max_d) {
            max_d = d;
        }
    }

    return max_d;
}
