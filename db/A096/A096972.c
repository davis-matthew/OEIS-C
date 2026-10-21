/*
 * A096972  Number of preimages of n (or immediate predecessors) under map
 *          f(k) = k + (product of nonzero digits of k).
 * https://oeis.org/A096972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = # of integers k < n such that k +
 *            product_of_nonzero_digits(k) = n
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation stores a table of preimage counts indexed by the target value. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Array of counts, indexed by the integer target value. */
static seq_t *preimage_counts = NULL;
/* Largest k that has been processed (k contributes to a(k+1) at most). */
static idx_t computed_k = 0;
/* Current capacity of the preimage_counts array (in terms of indices). */
static idx_t array_size = 0;

/* Ensure that preimage_counts has at least (idx + 1) entries. */
static void ensure_size(idx_t idx)
{
    if (idx < array_size) return;
    size_t newsize = array_size ? (size_t)array_size * 2 : 1;
    while (newsize <= (size_t)idx) newsize *= 2;

    seq_t *newarr = (seq_t *)malloc(newsize * sizeof(seq_t));
    if (!newarr) exit(EXIT_FAILURE);

    if (preimage_counts) {
        memcpy(newarr, preimage_counts, array_size * sizeof(seq_t));
        free(preimage_counts);
    }
    memset(newarr + array_size, 0, (newsize - array_size) * sizeof(seq_t));

    preimage_counts = newarr;
    array_size = (idx_t)newsize;
}

/* Compute product of non‑zero decimal digits of an unsigned integer value. */
static idx_t digit_product_idx(idx_t x)
{
    idx_t prod = 1;
    while (x > 0) {
        idx_t d = x % 10;
        x /= 10;
        if (d != 0) prod *= d;
    }
    return prod;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* terms below the offset are undefined */

    /* Ensure storage for a(n) and all targets that might occur. */
    ensure_size(n);

    /* Process all k from computed_k+1 up to n-1, updating preimage counts. */
    while (computed_k < n - 1) {
        ++computed_k;
        idx_t prod = digit_product_idx(computed_k);
        idx_t target = computed_k + prod;
        if (target >= array_size) ensure_size(target);
        preimage_counts[target] = preimage_counts[target] + (seq_t)1;
    }

    return preimage_counts[n];
}
