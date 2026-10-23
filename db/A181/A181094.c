/*
 * A181094  Permutation of positive integers created from sequence A000040(n)
 *          = primes.
 * https://oeis.org/A181094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = permutation of positive integers created from primes
 *            Algorithm: start with identity permutation a[i]=i. For each
 *            prime p in increasing order, find the smallest index i that has
 *            still a[i]==i (i is the first untouched position). Swap a[i]
 *            with a[p]. After this swap, mark both i and p as no...
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_alloc_array.h"

static seq_t *perm = NULL;              /* 1‑based permutation array   */
static bool *is_id = NULL;              /* flag whether a[i]==i        */
static size_t perm_len = 0;              /* capacity of arrays          */

static uint32_t *prime_list = NULL;      /* list of primes <= perm_len  */
static size_t prime_count = 0;           /* number of primes stored     */

static idx_t next_prime_index = 0;       /* next prime to use           */
static idx_t next_identity_index = 1;    /* smallest index still identity */

/* one‑time initialisation */
static void init(void)
{
    if (perm != NULL) return;

    perm_len = 1000000;                    /* ample for n ≤ 10000 */

    perm = seq_alloc_array(perm_len);
    for (size_t i = 1; i < perm_len; ++i) perm[i] = (seq_t)i;

    is_id = (bool*)malloc(perm_len * sizeof(bool));
    for (size_t i = 0; i < perm_len; ++i) is_id[i] = true;
    is_id[0] = false;                     /* unused index 0 */

    /* sieve of Eratosthenes to list primes up to perm_len */
    bool *prime_flag = (bool*)malloc((perm_len + 1) * sizeof(bool));
    for (size_t i = 0; i <= perm_len; ++i) prime_flag[i] = true;
    prime_flag[0] = prime_flag[1] = false;
    for (size_t i = 2; i * i <= perm_len; ++i)
        if (prime_flag[i])
            for (size_t j = i * i; j <= perm_len; j += i)
                prime_flag[j] = false;

    prime_list = (uint32_t*)malloc(perm_len * sizeof(uint32_t));
    prime_count = 0;
    for (size_t i = 2; i <= perm_len; ++i)
        if (prime_flag[i]) prime_list[prime_count++] = (uint32_t)i;
    free(prime_flag);
}

/* process swaps until all indices ≤ n have been dealt with */
static void ensure_processed(idx_t n)
{
    while (next_identity_index <= n) {
        uint32_t p = prime_list[next_prime_index++];
        idx_t i = next_identity_index;

        /* swap a[i] and a[p] */
        seq_t tmp = perm[i];
        perm[i] = perm[p];
        perm[p] = tmp;

        is_id[i] = false;
        is_id[p] = false;

        /* advance to next untouched index */
        do { ++next_identity_index; }
        while (next_identity_index < perm_len &&
               !is_id[(size_t)next_identity_index]);
    }
}

seq_t a(idx_t n)
{
    init();
    ensure_processed(n);
    return perm[n];
}
