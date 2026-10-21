/*
 * A096971  G.f. satisfies: A(x) = A( x^2*A000108(x^2) )*x*A000984(x^2), where
 *          A000108(x) is the g.f. for the Catalan sequence and A000984(x) =
 *          d/dx x*A000108(x).
 * https://oeis.org/A096971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..floor((n+1)/2)} C(2n-2j-1, n-1) * a(j); a(0)=1,
 *            a(1)=1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binom.h"

static seq_t *seq_array = NULL;   /* cache of already computed terms */
static idx_t  seq_cap   = 0;      /* current capacity of seq_array */
static idx_t  seq_len   = 0;      /* number of valid entries in seq_array */

/* Ensure seq_array can address indices up to "need" (inclusive). */
static void seq_grow(idx_t need)
{
    if (seq_cap <= need) {
        idx_t new_cap = seq_cap ? seq_cap << 1 : 8;
        while (new_cap <= need) new_cap <<= 1;
        seq_array = (seq_t *)realloc(seq_array, new_cap * sizeof(seq_t));
        seq_cap   = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Initialise base cases once */
    if (seq_len == 0) {
        seq_grow(1);                 /* capacity at least 2 */
        seq_array[0] = (seq_t)1;
        seq_array[1] = (seq_t)1;
        seq_len      = 2;
    }

    /* If the requested term is already known, return it immediately. */
    if (n < seq_len) return seq_array[n];

    /* Compute all missing terms up to n. */
    seq_grow(n);                    /* ensure enough space */

    for (idx_t k = seq_len; k <= n; ++k) {
        seq_t sum = 0;
        if (k >= 2) {
            idx_t j_max = (k + 1) >> 1;          /* floor((k+1)/2) */
            for (idx_t j = 0; j <= j_max; ++j) {
                idx_t a_val = 2 * k - 2 * j - 1;
                idx_t b_val = k - 1;
                seq_t comb  = binom(a_val, b_val);
                sum += comb * seq_array[j];
            }
        }
        seq_array[k] = sum;
    }
    seq_len = n + 1;
    return seq_array[n];
}
