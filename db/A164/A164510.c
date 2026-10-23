/*
 * A164510  First differences of A071904 (Odd composite numbers).
 * https://oeis.org/A164510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = odd composite(n+1) - odd composite(n) * * Computes
 *            A164510: the first differences of odd composite numbers. * The
 *            sequence is generated lazily; the result for a given n is *
 *            cached between calls. The algorithm repeatedly finds the next
 *            odd * composite number using a primality test from the...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"

static seq_t *diff_arr = NULL;     /* stored differences */
static size_t diff_cap = 0;        /* current allocated capacity */
static size_t diff_len = 0;        /* number of differences stored so far */

/* The last odd composite seen.  0 indicates not yet initialized. */
static uint64_t last_comp = 0;

/* Ensure the difference array can hold at least `needed` elements. */
static void ensure_diff_cap(size_t needed)
{
    if (needed <= diff_cap)
        return;
    size_t new_cap = diff_cap ? diff_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_arr) /* allocation failure – shouldn't happen in test env */
        return;
    if (diff_arr) {
        /* Preserve already computed differences. */
        memcpy(new_arr, diff_arr, diff_len * sizeof(seq_t));
        free(diff_arr);
    }
    diff_arr = new_arr;
    diff_cap = new_cap;
}

/* Extend the pre‑computed list of differences until it contains `n`
 * elements (i.e. differences for indices 1…n). */
static void extend_to(idx_t n)
{
    if (n == 0)
        return;

    /* Initialise the first odd composite if we have not yet. */
    if (last_comp == 0) {
        uint64_t cand = 3;
        while (true) {
            cand += 2; /* iterate over odd numbers only */
            if (!is_prime((idx_t)cand)) {
                last_comp = cand;
                break;
            }
        }
    }

    while (diff_len < (size_t)n) {
        /* Search for the next odd composite. */
        uint64_t cand = last_comp + 2;
        while (true) {
            if (!is_prime((idx_t)cand)) {
                /* Record the difference. */
                seq_t diff = (seq_t)(cand - last_comp);
                ensure_diff_cap(diff_len + 1);
                diff_arr[diff_len] = diff;
                ++diff_len;
                last_comp = cand;
                break;
            }
            cand += 2;
        }
    }
}

/* Public function implementing OEIS A164510. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined for offset 1, but safe */
    extend_to(n);                   /* needs at least n differences */
    return diff_arr[n - 1];         /* a(1) corresponds to diff_arr[0] */
}
