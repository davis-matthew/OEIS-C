/*
 * A048989  Numbers k such that pi(k) is prime.
 * https://oeis.org/A048989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 8184, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A048989: numbers k such that pi(k) is prime
   Computation: sieve all primes up to a fixed bound LIMIT_K,
   then record each k whose running prime count pi(k) is prime.
   The resulting array is cached statically so that a(idx_t n)
   can be answered in O(1) after the first call. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define LIMIT_K ((size_t)3000000)   /* bound sufficient for n <= 10000 */

/* static storage for the sieve, the prime list and the answer list */
static bool *is_comp = NULL;                 /* 0 = prime, 1 = composite */
static uint64_t *prime_list = NULL;
static size_t prime_cnt = 0;
static seq_t *good = NULL;
static size_t good_cnt = 0;

/* forward declarations */
static void build_sequence(void);

/* external function required by the exercise */
seq_t a(idx_t n)
{
    if (good_cnt == 0)           /* first call, build everything */
        build_sequence();

    /* indices in OEIS start at 1 */
    if (n == 0 || n > good_cnt)
        return (seq_t)0;           /* out of bounds; behaviour undefined */

    return good[n - 1];
}

/* ------------------------------------------------------------ */

static void build_sequence(void)
{
    /* --- 1. Sieve primes up to LIMIT_K ------------------------------------ */
    is_comp = (bool *)malloc((LIMIT_K + 1) * sizeof(bool));
    if (!is_comp) exit(1);
    for (size_t i = 0; i <= LIMIT_K; ++i)
        is_comp[i] = false;
    if (LIMIT_K >= 0) is_comp[0] = true;
    if (LIMIT_K >= 1) is_comp[1] = true;

    for (size_t p = 2; p * p <= LIMIT_K; ++p) {
        if (!is_comp[p]) {
            for (size_t j = p * p; j <= LIMIT_K; j += p)
                is_comp[j] = true;
        }
    }

    /* collect primes into an array */
    prime_cnt = 0;
    for (size_t i = 2; i <= LIMIT_K; ++i)
        if (!is_comp[i])
            prime_cnt++;

    prime_list = (uint64_t *)malloc(prime_cnt * sizeof(uint64_t));
    if (!prime_list) exit(1);
    size_t idx = 0;
    for (size_t i = 2; i <= LIMIT_K; ++i)
        if (!is_comp[i])
            prime_list[idx++] = i;

    /* --- 2. Build the list of k such that pi(k) is prime ----------------- */
    good_cnt = 0;
    good = NULL;
    size_t good_cap = 0;
    size_t pi_cnt = 0;          /* running prime count */
    for (size_t k = 1; k <= LIMIT_K; ++k) {
        if (!is_comp[k])        /* prime */
            ++pi_cnt;

        if (pi_cnt >= 2 && !is_comp[pi_cnt]) { /* pi(k) is prime */
            if (good_cnt == good_cap) {
                good_cap = good_cap == 0 ? 512 : good_cap * 2;
                good = (seq_t *)realloc(good, good_cap * sizeof(seq_t));
                if (!good) exit(1);
            }
            good[good_cnt++] = (seq_t)k;
        }
    }
}
