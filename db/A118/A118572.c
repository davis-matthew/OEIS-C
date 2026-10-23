/*
 * A118572  Sophie Germain primes for which the product of the digits is
 *          square.
 * https://oeis.org/A118572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 202, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_product, floor_sqrt, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the n-th Sophie Germain prime for which the product of its
 *              digits is a perfect square.  The sequence is found by brute‑force
 *              search with memoisation; each term is added to a static array
 *              when it is discovered.
 */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/digit_product.h"
#include "../utils/floor_sqrt.h"

/* Return the n‑th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    /* Static cache of found terms. */
    static seq_t *sg = NULL;      /* array of found SG primes */
    static size_t sg_count = 0;   /* number of stored terms */
    static size_t sg_capacity = 0;/* allocated capacity */
    static idx_t last_checked = 1;/* next integer to test */

    /* Allocate initial cache on first call. */
    if (sg == NULL) {
        sg_capacity = 64;
#ifdef __cplusplus
        sg = reinterpret_cast<seq_t*>(malloc(sg_capacity * sizeof(seq_t)));
#else
        sg = malloc(sg_capacity * sizeof(seq_t));
#endif
        if (sg == NULL) return 0;   /* allocation failure */
    }

    /* Extend cache until we have n terms. */
    while (sg_count < (size_t)n) {
        ++last_checked;

        /* Skip even numbers greater than 2. */
        if (last_checked > 2 && (last_checked & 1) == 0)
            continue;

        /* Sophie Germain condition: p and 2p+1 prime. */
        if (!is_prime((idx_t)last_checked))
            continue;
        idx_t twoP1 = 2 * last_checked + 1;
        if (!is_prime(twoP1))
            continue;

        /* Product of digits must be a perfect square. */
        seq_t prod = digit_product((seq_t)last_checked);
        seq_t sq = floor_sqrt(prod);
        if (sq * sq != prod)
            continue;

        /* Store the found prime. */
        if (sg_count >= sg_capacity) {
            sg_capacity *= 2;
#ifdef __cplusplus
            seq_t *tmp = reinterpret_cast<seq_t*>(realloc(sg, sg_capacity * sizeof(seq_t)));
#else
            seq_t *tmp = realloc(sg, sg_capacity * sizeof(seq_t));
#endif
            if (tmp == NULL) return 0;   /* allocation failure */
            sg = tmp;
        }
        sg[sg_count++] = (seq_t)last_checked;
    }

    return sg[n - 1];
}
