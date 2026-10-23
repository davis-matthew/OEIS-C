/*
 * A100466  Semiprimes of special form: sum of an integer k and the k-th
 *          semiprime.
 * https://oeis.org/A100466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the n-th semiprime of the form k + (k‑th semiprime)
 * Range:     u8 n <= 21, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   add_seq, is_semiprime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* i.e. a(n) = k + s_k where s_k is the k‑th semiprime and the sum itself
   must also be a semiprime.  The sequence is listed in increasing order,
   offset 1.  The implementation grows the required data lazily and
   remembers the results between calls. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/add_seq.h"
#include "../utils/is_semiprime_uint64.h"

static seq_t *semiprimes = NULL;   /* list of semiprimes (1‑based index) */
static size_t sem_cap   = 0;
static size_t sem_cnt    = 0;

static seq_t *sums      = NULL;    /* list of valid a(k) values (sorted) */
static size_t sum_cap   = 0;
static size_t sum_cnt    = 0;

static idx_t  next_candidate = 4;   /* next integer to test for semiprime */

/* Ensure at least 'need' elements in the semiprimes array.  We test all
   integers starting from next_candidate until we have enough. */
static void ensure_semiprimes(size_t need)
{
    while (sem_cnt < need) {
        if (is_semiprime_uint64((uint64_t)next_candidate))
            add_seq(&semiprimes, &sem_cap, &sem_cnt, (seq_t)next_candidate);
        next_candidate++;   /* advance to next integer */
    }
}

/* Generate additional sums until we have at least 'need' of them.  We
   process k values sequentially, starting from 1, using the semiprime
   list already built.  A sum is added only if it is itself a semiprime. */
static void ensure_sums(size_t need)
{
    static idx_t k_processed = 0;   /* number of k's already examined */

    while (sum_cnt < need) {
        /* Ensure we have the semiprime needed for this k */
        ensure_semiprimes((size_t)(k_processed + 1));

        seq_t sk   = semiprimes[k_processed];           /* s_k */
        seq_t sum  = (seq_t)k_processed + 1 + sk;      /* k + s_k, k is 0‑based */

        if (is_semiprime_uint64((uint64_t)sum))
            add_seq(&sums, &sum_cap, &sum_cnt, sum);

        k_processed++;    /* move to next k */
    }
}

seq_t a(idx_t n)
{
    if (n == 0)   /* offset 1; n==0 is undefined, return 0 */
        return 0;

    /* Compute sums until we have at least n of them */
    ensure_sums((size_t)n);

    return sums[(size_t)n - 1];   /* 0‑based indexing */
}
