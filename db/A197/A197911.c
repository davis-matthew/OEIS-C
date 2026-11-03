/*
 * A197911  Representable by A001045 (Jacobsthal sequence). Complement of
 *          A003158.
 * https://oeis.org/A197911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 191, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9269 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A197911: a(n) is the n-th smallest integer that can be expressed
 * as a sum of distinct Jacobsthal numbers J_k (k≥1). This implementation
 * generates all subset sums of the distinct Jacobsthal numbers incrementally
 * until it has at least n+1 elements. */

#include <stdlib.h>
#include "../utils/mem.h"

/* Merge two sorted unique arrays into a new sorted unique array.
 * old: original subset sums array (size old_sz)
 * add: offset value to add to each element of old, producing 'new_sums'
 * Returns pointer to new array and sets new_sz accordingly. */
static seq_t *merge_with_offset(seq_t *old, size_t old_sz, seq_t add, size_t *new_sz)
{
    /* Create new_sums = old + add */
    seq_t *new_sums = mem_new(seq_t, old_sz);
    for (size_t i = 0; i < old_sz; ++i)
        new_sums[i] = old[i] + add;

    /* Allocate maximum possible size for merged array */
    seq_t *merged = mem_new(seq_t, (old_sz * 2));

    size_t i = 0, j = 0, k = 0;
    while (i < old_sz && j < old_sz) {
        seq_t a = old[i];
        seq_t b = new_sums[j];
        if (a < b) {
            merged[k++] = a;
            ++i;
        } else if (b < a) {
            merged[k++] = b;
            ++j;
        } else {         /* a == b */
            merged[k++] = a;
            ++i; ++j;
        }
    }
    while (i < old_sz)
        merged[k++] = old[i++];
    while (j < old_sz)
        merged[k++] = new_sums[j++];

    *new_sz = k;
    mem_free(new_sums);
    return merged;
}

/* Internal helper: generate next Jacobsthal number */
static inline seq_t next_jacobsthal(void)
{
    /* J(n) = J(n-1) + 2 * J(n-2)  with J(0)=0, J(1)=1 */
    static seq_t j_prev2 = 0;
    static seq_t j_prev1 = 1;
    seq_t j_next = j_prev1 + (j_prev2 << 1);
    j_prev2 = j_prev1;
    j_prev1 = j_next;
    return j_next;
}

/* Forward declaration of the function required by the build system */
seq_t a(idx_t n);

/* Main function to compute the n-th term of A197911 */
seq_t a(idx_t n)
{
    /* Static state: store all computed subset sums */
    static seq_t *sums = NULL;       /* array of sums */
    static size_t sums_sz = 0;       /* current number of sums stored */
    static int initialized = 0;      /* flag */

    /* Initialize on first call */
    if (!initialized) {
        /* start with 0 (empty sum) */
        sums = mem_new(seq_t, 1);
        sums[0] = 0;
        sums_sz = 1;
        initialized = 1;
    }

    /* Ensure we have at least n+1 sums (since n is zero‑based) */
    while (sums_sz <= (size_t)n) {
        seq_t coin = next_jacobsthal();        /* next Jacobsthal number */

        /* Skip duplicates (only 1 appears twice in the sequence) */
        static seq_t last_coin = 0;
        while (coin == last_coin) {
            coin = next_jacobsthal();
        }
        last_coin = coin;

        /* Merge current sums with new sums offset by 'coin' */
        size_t new_sz = 0;
        seq_t *new_sums = merge_with_offset(sums, sums_sz, coin, &new_sz);

        /* Replace old array */
        mem_free(sums);
        sums = new_sums;
        sums_sz = new_sz;
    }

    return sums[(size_t)n];
}
