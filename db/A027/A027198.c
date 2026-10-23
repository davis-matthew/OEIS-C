/*
 * A027198  Number of partitions of n into an even number of parts, the least
 *          being 6; also, a(n+6) = number of partitions of n into an odd
 *          number of parts, each >=6.
 * https://oeis.org/A027198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into an even number of parts,
 *            the least part being exactly 6. Equivalently: a(n) = the number
 *            of partitions of (n-6) into parts >= 6 with an odd number of
 *            parts
 * Range:     u8 n <= 54, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static idx_t precomputed_max = 0;

#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> even_vec;
static std::vector<seq_t> odd_vec;

static void recompute(idx_t upto) {
    even_vec.assign(upto + 1, seq_t());
    odd_vec.assign(upto + 1, seq_t());
    even_vec[0] = seq_t(1);
    for (idx_t j = 6; j <= upto; ++j) {
        for (idx_t s = j; s <= upto; ++s) {
            even_vec[s] += odd_vec[s - j];
            odd_vec[s] += even_vec[s - j];
        }
    }
    precomputed_max = upto;
}
#else
static seq_t *even_arr = NULL;
static seq_t *odd_arr  = NULL;

static void recompute(idx_t upto) {
    seq_t *new_even = (seq_t *)calloc((size_t)(upto + 1), sizeof(seq_t));
    seq_t *new_odd  = (seq_t *)calloc((size_t)(upto + 1), sizeof(seq_t));
    if (new_even == NULL || new_odd == NULL) {
        /* OOM – abort to be safe */
        abort();
    }
    new_even[0] = (seq_t)1;
    for (idx_t j = 6; j <= upto; ++j) {
        for (idx_t s = j; s <= upto; ++s) {
            new_even[s] += new_odd[s - j];
            new_odd[s]  += new_even[s - j];
        }
    }
    if (even_arr) free(even_arr);
    if (odd_arr)  free(odd_arr);
    even_arr = new_even;
    odd_arr  = new_odd;
    precomputed_max = upto;
}
#endif

seq_t a(idx_t n) {
    if (n < 12) return (seq_t)0;
    idx_t target = n - 6;
    if (target > precomputed_max) recompute(target);
#ifdef __cplusplus
    return odd_vec[target];
#else
    return odd_arr[target];
#endif
}
