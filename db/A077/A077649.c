/*
 * A077649  Initial digit of composite numbers.
 * https://oeis.org/A077649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the first decimal digit of the n-th composite number
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-03)
 * Helpers:   is_prime_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/is_prime_seq.h"
#include "../utils/mem.h"

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static seq_t *composites = NULL;
static size_t comp_cap   = 0;
static size_t comp_cnt   = 0;   /* number of composites stored          */
static idx_t  curr_num   = 3;   /* last integer tested; start at 3      */

/* Return the most significant decimal digit of x. */
static inline seq_t leading_digit(seq_t x)
{
    while (x >= 10) x /= 10;
    return x;
}

/* Grow the composite cache until it contains at least n terms. */
static void ensure_composites(idx_t n)
{
    while (comp_cnt < (size_t)n) {
        curr_num++;                                 /* next integer to test   */
        if (!is_prime_seq((seq_t)curr_num)) {        /* composite?             */
            if (comp_cnt == comp_cap) {              /* enlarge buffer         */
                size_t new_cap = comp_cap ? comp_cap*2 : 64;
                composites = seq_realloc(composites, comp_cap, new_cap);
                comp_cap = new_cap;
            }
            composites[comp_cnt++] = (seq_t)curr_num; /* store composite        */
        }
    }
}

/* External sequence function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* not used for the official offset   */
    ensure_composites(n);                /* make sure we have the n-th term     */
    return leading_digit(composites[n-1]);/* compute its first decimal digit     */
}
