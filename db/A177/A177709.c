/*
 * A177709  Sums of 4 distinct primorials.
 * https://oeis.org/A177709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the n-th smallest sum of four distinct primorials
 *            (A002110). That is a(n) = p[i] + p[j] + p[k] + p[l] where 1 ≤ i
 *            < j < k < l are indices of primorials and the sums are sorted in
 *            ascending order
 * Range:     u8 n <= 5, u16 n <= 35, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   cmp_seq_t, free_seq_array, seq_alloc_array, sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/sieve_first_n_primes.h"   /* sieve_first_n_primes */
#include "../utils/seq_alloc_array.h"       /* seq_alloc_array */
#include "../utils/free_seq_array.h"        /* free_seq_array */
#include "../utils/cmp_seq_t.h"             /* cmp_seq_t */

static seq_t *sums = NULL;     /* sorted sums */
static size_t sums_len = 0;    /* current number of sums stored */
static idx_t last_requested = 0; /* largest n for which we already computed */

/* Ensure that the array `sums` contains at least `need` entries
   (which will be the first `need` terms of the sequence). */
static void ensure_sufficient(idx_t need)
{
    if (need <= last_requested)
        return;                  /* already enough computed */

    /* Find the minimal index M such that C(M,4) >= need. */
    idx_t M = 4;
    while ((M * (M - 1) * (M - 2) * (M - 3)) / 24 < need)
        ++M;

    /* Allocate primorial array of size M+1 (indices 1..M). */
    seq_t *prim = seq_alloc_array((size_t)M + 1);
    if (!prim)
        return; /* memory failure – caller will receive garbage */

    /* Get the first M-1 primes (needed to build primorials). */
    uint32_t *prime_list = NULL;
    size_t primes_cnt = 0;
    sieve_first_n_primes(M - 1, &prime_list, &primes_cnt);
    if (!prime_list) {
        free_seq_array(prim);
        return;
    }

    /* prim[1] = 1; prim[i] = prim[i-1] * prime[i-2] for i ≥ 2. */
    prim[1] = (seq_t)1;
    for (idx_t i = 2; i <= M; ++i)
        prim[i] = prim[i - 1] * (seq_t)prime_list[i - 2];

    /* Number of distinct 4‑tuples from 1..M. */
    size_t need_sums = (M * (M - 1) * (M - 2) * (M - 3)) / 24;

    /* Allocate (or re‑allocate) the sums array. */
    if (sums)
        free_seq_array(sums);
    sums = seq_alloc_array(need_sums);
    if (!sums) {
        free_seq_array(prim);
        free(prime_list);
        return;
    }

    /* Generate all sums of four distinct indices. */
    size_t pos = 0;
    for (idx_t i = 1; i <= M - 3; ++i) {
        for (idx_t j = i + 1; j <= M - 2; ++j) {
            for (idx_t k = j + 1; k <= M - 1; ++k) {
                for (idx_t l = k + 1; l <= M; ++l) {
                    sums[pos++] = prim[i] + prim[j] + prim[k] + prim[l];
                }
            }
        }
    }

    /* Sort the sums.  cmp_seq_t is provided by the utilities. */
    qsort(sums, need_sums, sizeof(seq_t), cmp_seq_t);

    /* Record state for future calls. */
    sums_len = need_sums;
    last_requested = need_sums;

    /* Clean up. */
    free_seq_array(prim);
    free(prime_list);
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n = 0, but safe */

    ensure_sufficient(n);          /* ensure we have at least n terms */

    /* n is 1‑based; the array is 0‑based. */
    return sums[n - 1];
}
