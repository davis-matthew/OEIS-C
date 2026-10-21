/*
 * A204814  Number of decompositions of 2n into an unordered sum of two
 *          Ramanujan primes.
 * https://oeis.org/A204814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered representations of 2·n as a sum of
 *            two Ramanujan primes. The implementation builds a sieve,
 *            computes Ramanujan primes up to the needed limit, and counts
 *            unordered pairs that sum to 2·n. The routine is valid C11 and
 *            C++17, using only integer arithmetic
 * Range:     u8 n <= 228, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   u16_min
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/u16_min.h"

static idx_t   built_limit   = 0;    /* highest value for which tables exist */

/* Prime sieve and auxiliary data */
static uint8_t   *is_prime  = NULL;   /* 0/1 array */
static uint32_t *pi_arr    = NULL;   /* prefix prime counts */
static uint16_t *diff_arr  = NULL;   /* pi(x) - pi(x/2) */
static uint16_t *suff_min  = NULL;   /* suffix minimum of diff_arr */

/* Ramanujan primes */
static uint32_t *ram_list  = NULL;
static size_t   ram_cnt    = 0;
static size_t   ram_cap    = 0;

/* Build tables up to at least 'limit' (inclusive). */
static void ensure_limit(idx_t limit)
{
    if (limit <= built_limit)
        return;

    /* Free old data if present */
    free(is_prime);   free(pi_arr);   free(diff_arr);
    free(suff_min);   free(ram_list);

    /* Allocate arrays */
    size_t M = (size_t)limit;
    is_prime = (uint8_t *)malloc((M + 1) * sizeof(uint8_t));
    pi_arr   = (uint32_t *)malloc((M + 1) * sizeof(uint32_t));
    diff_arr = (uint16_t *)malloc((M + 1) * sizeof(uint16_t));
    suff_min = (uint16_t *)malloc((M + 2) * sizeof(uint16_t));

    if (!is_prime || !pi_arr || !diff_arr || !suff_min)
        exit(EXIT_FAILURE);

    /* Sieve of Eratosthenes */
    for (size_t i = 0; i <= M; ++i) is_prime[i] = 1;
    if (M >= 0) is_prime[0] = 0;
    if (M >= 1) is_prime[1] = 0;
    for (size_t i = 2; i * i <= M; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= M; j += i)
                is_prime[j] = 0;
        }
    }

    /* Build prefix prime counts */
    pi_arr[0] = 0;
    for (size_t i = 1; i <= M; ++i)
        pi_arr[i] = pi_arr[i - 1] + (uint32_t)is_prime[i];

    /* Compute diff array */
    for (size_t i = 0; i <= M; ++i)
        diff_arr[i] = (uint16_t)(pi_arr[i] - pi_arr[i / 2]);

    /* Compute suffix minimum of diff */
    suff_min[M + 1] = UINT16_MAX;
    for (size_t i = M + 1; i-- > 0; )   /* i from M down to 0 */
        suff_min[i] = u16_min(diff_arr[i], suff_min[i + 1]);

    /* Identify Ramanujan primes */
    ram_cnt = 0;
    ram_cap = 32;  /* initial capacity */
    ram_list = (uint32_t *)malloc(ram_cap * sizeof(uint32_t));
    if (!ram_list) exit(EXIT_FAILURE);

    for (size_t i = 2; i <= M; ++i) {
        if (!is_prime[i]) continue;
        if (diff_arr[i] == suff_min[i]) {
            if (ram_cnt == ram_cap) {
                ram_cap <<= 1;
                ram_list = (uint32_t *)realloc(ram_list,
                                              ram_cap * sizeof(uint32_t));
                if (!ram_list) exit(EXIT_FAILURE);
            }
            ram_list[ram_cnt++] = (uint32_t)i;
        }
    }

    built_limit = limit;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                            */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used, safety */

    idx_t target = 2 * n;           /* even number to be summed */
    if (target > built_limit || built_limit == 0)
        ensure_limit(target * 2 + 100);  /* sufficient growth buffer */

    seq_t count = 0;
    for (size_t i = 0; i < ram_cnt && ram_list[i] <= target; ++i) {
        uint32_t p = ram_list[i];
        for (size_t j = i; j < ram_cnt && ram_list[j] <= target; ++j) {
            uint32_t q = ram_list[j];
            seq_t sum = (seq_t)p + (seq_t)q;
            if (sum == target)
                ++count;
            else if (sum > target)
                break;
        }
    }
    return count;
}
