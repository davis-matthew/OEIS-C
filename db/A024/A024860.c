/*
 * A024860  a(n) = s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n-k+1), where k = [ n/2
 *          ], s = (natural numbers), t = (composite numbers).
 * https://oeis.org/A024860
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{i=1}^{⌊n/2⌋} i * t(n-i+1) where t(k) is the k‑th
 *            composite number (4,6,8,9,10,12,…)
 * Range:     u8 n <= 11, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_small.h"

/* static cache of composite numbers (1‑based list) */
static seq_t *comp = NULL;
static size_t comp_size = 0;  /* number of composites stored */
static size_t comp_cap  = 0;  /* allocated capacity */

/* Ensure that the cache contains at least 'need' composite numbers. */
static void ensure_composites(idx_t need)
{
    if (need <= (idx_t)comp_size)
        return;  /* already enough */

    /* Grow the array if necessary. */
    if (comp_cap < (size_t)need) {
        size_t new_cap = comp_cap ? comp_cap * 2 : 256;
        if (new_cap < (size_t)need)
            new_cap = (size_t)need;
        seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!tmp) abort();                 /* allocation failure */
        if (comp) {
            memcpy(tmp, comp, comp_size * sizeof(seq_t));
            free(comp);
        }
        comp = tmp;
        comp_cap = new_cap;
    }

    /* Start searching from the next integer after the last known composite. */
    idx_t last_comp = comp_size ? (idx_t)comp[comp_size - 1] : 3; /* 4 will be next */
    idx_t cand = last_comp + 1;

    while (comp_size < (size_t)need) {
        if (cand > 1 && !is_prime_small(cand)) {
            comp[comp_size++] = (seq_t)cand;
        }
        cand++;
    }
}

seq_t a(idx_t n)
{
    if (n < 2)            /* offset is 2 */
        return 0;

    idx_t k = n / 2;             /* floor(n/2) */
    ensure_composites(n);        /* need composites up to index n */

    seq_t sum = 0;
    for (idx_t i = 1; i <= k; ++i) {
        idx_t idx = n - i + 1;                  /* composite index (1‑based) */
        seq_t tval = comp[idx - 1];             /* zero‑based array */
        seq_t i_t = (seq_t)i;
        sum += i_t * tval;
    }
    return sum;
}
