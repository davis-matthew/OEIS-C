/*
 * A028197  Expansion of 1/((1-5x)(1-9x)(1-10x)(1-12x)).
 * https://oeis.org/A028197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A028197
   Expansion of 1/((1-5x)(1-9x)(1-10x)(1-12x))
   a(0) = 1, a(1) = 36
   a(n) = 22*a(n-1) - 120*a(n-2) + (9^(n+1) - 5^(n+1))/4      (n ≥ 2) */
 
#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* trivial cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)36;

    /* static state holding computed values */
    static seq_t *seq_arr   = NULL;
    static seq_t *pow5      = NULL;
    static seq_t *pow9      = NULL;
    static size_t  seq_cap  = 0;   /* capacity of seq_arr         */
    static size_t  pow_cap  = 0;   /* capacity of pow arrays       */
    static idx_t   max_comp = 1;   /* highest index computed in seq_arr */
    static idx_t   last_pow = 0;   /* highest index computed in pow arrays */

    /* ensure enough capacity for seq_arr up to n */
    size_t need_seq = (size_t)n + 1;         /* indices 0..n         */
    size_t need_pow = (size_t)n + 3;         /* indices 0..n+1 needed */

    if (need_seq > seq_cap) {
        seq_arr = seq_realloc(seq_arr, seq_cap, need_seq);
        seq_cap = need_seq;
    }

    if (need_pow > pow_cap) {
        pow5 = seq_realloc(pow5, pow_cap, need_pow);
        pow9 = seq_realloc(pow9, pow_cap, need_pow);
        pow_cap = need_pow;
    }

    /* initialisation on first entry */
    if (max_comp == 1) {
        seq_arr[0] = (seq_t)1;
        seq_arr[1] = (seq_t)36;
        pow5[0] = (seq_t)1;
        pow9[0] = (seq_t)1;
    }

    /* make sure powers up to exponent (n+1) exist */
    while (last_pow < (idx_t)n + 1) {
        ++last_pow;
        pow5[last_pow] = pow5[last_pow - 1] * (seq_t)5;
        pow9[last_pow] = pow9[last_pow - 1] * (seq_t)9;
    }

    /* compute missing terms of the sequence */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        /* (9^(i+1) - 5^(i+1))/4 */
        seq_t diff = (pow9[i + 1] - pow5[i + 1]) / (seq_t)4;

        /* 22*a(i-1) + diff - 120*a(i-2) */
        seq_t term = seq_arr[i - 1] * (seq_t)22;
        term += diff;
        term -= seq_arr[i - 2] * (seq_t)120;

        seq_arr[i] = term;
    }

    max_comp = n;
    return seq_arr[n];
}
