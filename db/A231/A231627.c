/*
 * A231627  Numbers k such that sigma(sigma(k)) - sigma(k) - 1 is prime.
 * https://oeis.org/A231627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 1640, u32 n <= 1962, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A231627: numbers k such that σ(σ(k)) − σ(k) − 1 is prime.
   The implementation searches k sequentially, keeps a cache of found
   terms and returns the (n‑1)-th entry (1‑based indexing). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/sigma_of.h"      /* sigma_of(idx_t n) -> seq_t */
#include "../utils/is_prime_big.h"  /* is_prime_big(seq_t n) -> bool */

static seq_t *list  = NULL;
static size_t sz   = 0;
static size_t cap  = 0;
static idx_t last_k = 0;

static void grow_cap(void)
{
    size_t new_cap = (cap == 0) ? 64 : cap * 2;
    seq_t *tmp = (seq_t *)realloc(list, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* out of memory */
    list = tmp;
    cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Ensure cache is ready */
    if (!list) grow_cap();

    /* Find enough terms */
    while (sz < (size_t)n) {
        ++last_k;

        /* First sigma */
        seq_t s1 = sigma_of(last_k);

        /* We need to compute sigma(s1).  If s1 does not fit into idx_t,
           we cannot compute it with the provided utility and skip this k. */
        if (s1 > (seq_t)UINT64_MAX) continue;
        idx_t s1_idx = (idx_t)s1;

        /* Second sigma */
        seq_t s2 = sigma_of(s1_idx);

        /* Compute diff = σ(σ(k)) − σ(k) − 1 */
        if (s2 <= s1 + 1) continue;
        seq_t diff = s2 - s1 - 1;

        /* Check primality */
        if (diff >= 2 && is_prime_big(diff)) {
            /* Store the found k */
            if (sz == cap) grow_cap();
            list[sz] = (seq_t)last_k;
            ++sz;
        }
    }

    /* Return the desired term (1‑based index). */
    return list[n - 1];
}
