/*
 * A100409  Numbers n such that n is d_1*d_2*...*d_k-th composite number
 *          (n=A002808(d_1*d_2*...*d_k)) where d_1 d_2 ... d_k is the decimal
 *          expansion of n.
 * https://oeis.org/A100409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   digit_product, is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A100409
   a(n) = the composite number whose position among composites
          equals the product of its decimal digits (ignoring digits 0).
   Example: 98 is in the sequence because 9*8=72 and 98 is the
   72nd composite number. */

#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime_uint64.h"
#include "../utils/digit_product.h"

/* static state for memoisation */
static seq_t *terms      = NULL; /* a(1), a(2), … */
static size_t term_cnt   = 0, term_cap = 0;

static seq_t *comps      = NULL; /* 1st, 2nd, … composite numbers */
static size_t comp_cnt   = 0, comp_cap = 0;

static uint64_t last_checked = 3; /* last integer tested for composite */

/* ensure capacity of dynamic array */
static void ensure_term_cap(void)
{
    if (term_cnt == term_cap) {
        term_cap = term_cap == 0 ? 4 : term_cap << 1;
        terms = (seq_t *)realloc(terms, term_cap * sizeof(seq_t));
    }
}

static void ensure_comp_cap(void)
{
    if (comp_cnt == comp_cap) {
        comp_cap = comp_cap == 0 ? 4 : comp_cap << 1;
        comps = (seq_t *)realloc(comps, comp_cap * sizeof(seq_t));
    }
}

/* main function required by the problem */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for n=0 */

    /* extend the sequence until we have at least n terms */
    while (term_cnt < (size_t)n) {
        ++last_checked;
        if (last_checked < 4) continue;              /* composites start at 4 */
        if (is_prime_uint64((uint64_t)last_checked)) continue; /* skip primes */

        /* new composite found */
        ensure_comp_cap();
        comps[comp_cnt++] = (seq_t)last_checked;

        /* evaluate digit product */
        uint64_t prod = digit_product((seq_t)last_checked);
        if (prod == 0) continue;               /* product contains 0 -> cannot match */

        /* if product > number of composites seen, the dp‑th composite
           must be larger than current number, so equality impossible */
        if (prod == 0 || prod > comp_cnt) continue;

        /* check condition: does this composite occupy the product‑position? */
        if (comps[(size_t)prod - 1] == (seq_t)last_checked) {
            ensure_term_cap();
            terms[term_cnt++] = (seq_t)last_checked;
        }
    }
    return terms[(size_t)n - 1];
}
