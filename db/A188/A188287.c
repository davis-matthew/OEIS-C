/*
 * A188287  Convolution of A000085 with itself.
 * https://oeis.org/A188287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} A000085(k) * A000085(n-k)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 31, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* static cache for involution numbers A000085 */
static seq_t *inv_cache = NULL;  /* array of values 0..n */
static idx_t inv_len   = 0;      /* number of elements currently stored (inv_len = last_index+1) */
static idx_t inv_cap   = 0;      /* allocated capacity */

/* Ensure that inv_cache contains values 0..n (inclusive) */
static void ensure_inv(idx_t n)
{
    if (n < inv_len)
        return;  /* already computed */

    /* grow capacity: double until sufficient */
    idx_t new_cap = inv_cap ? inv_cap : 1;
    while (new_cap <= n)
        new_cap <<= 1;

    if (inv_cache == NULL) {
        inv_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    } else {
        seq_t *tmp = (seq_t *)realloc(inv_cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            /* Allocation failed – abort to avoid undefined behaviour */
            abort();
        }
        inv_cache = tmp;
    }
    inv_cap = new_cap;

    /* initialise from inv_len up to n */
    for (idx_t i = inv_len; i <= n; ++i) {
        if (i == 0) {
            inv_cache[0] = (seq_t)1;
        } else if (i == 1) {
            inv_cache[1] = (seq_t)1;
        } else {
            /* I(n) = I(n-1) + (n-1) * I(n-2) */
            inv_cache[i] = inv_cache[i - 1] + ((seq_t)(i - 1) * inv_cache[i - 2]);
        }
    }
    inv_len = n + 1;
}

/* Public function computing a(n) */
seq_t a(idx_t n)
{
    /* Convolution of involution numbers with itself */
    seq_t sum = 0;
    ensure_inv(n);
    for (idx_t k = 0; k <= n; ++k) {
        sum += inv_cache[k] * inv_cache[n - k];
    }
    return sum;
}
