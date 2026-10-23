/*
 * A161163  Indices of the isolated primes in the prime sequence, if indices
 *          are odd.
 * https://oeis.org/A161163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th odd index of an isolated prime (isolated primes are
 *            primes p such that both p−2 and p+2 are composite, i.e. the
 *            prime has no twin partners)
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compile with RESOURCES: include common utilities if needed. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static storage for computed data ------------------------------------------- */
static seq_t *indices = NULL;    /* array of odd indices of isolated primes   */
static size_t idx_count = 0;      /* number of stored indices                  */
static bool  *prime_flag = NULL;  /* sieve of Eratosthenes up to current limit*/
static uint64_t *prime_list = NULL; /* list of primes up to current limit      */
static size_t   prime_cnt = 0;    /* number of primes                         */
static idx_t current_limit = 0;   /* limit up to which primes were sieved     */

/* Helper: recompute all data up to a new maximum limit ----------------------- */
static void recompute_up_to(idx_t new_limit)
{
    /* free old data if any */
    if (prime_flag) free(prime_flag);
    if (prime_list) free(prime_list);
    if (indices)    free(indices);

    /* allocate sieve array (include a few extra slots for p+2 checks)  */
    size_t sieve_size = (size_t)new_limit + 3;          /* indices 0 .. new_limit+2   */
    prime_flag = (bool*)malloc(sieve_size * sizeof(bool));
    if (!prime_flag) return;                           /* memory failure guard       */
    for (size_t i = 0; i < sieve_size; ++i) prime_flag[i] = true;
    prime_flag[0] = prime_flag[1] = false;

    /* Eratosthenes */
    idx_t upper = new_limit + 2;
    for (idx_t i = 2; (size_t)i * (size_t)i <= (size_t)upper; ++i) {
        if (prime_flag[i]) {
            for (idx_t j = i * i; j <= upper; j += i) prime_flag[j] = false;
        }
    }

    /* Build list of primes up to new_limit */
    size_t cnt = 0;
    for (idx_t i = 2; i <= new_limit; ++i) if (prime_flag[i]) ++cnt;
    prime_list = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!prime_list) return; /* guard */
    prime_cnt = cnt;
    size_t pos = 0;
    for (idx_t i = 2; i <= new_limit; ++i) if (prime_flag[i]) prime_list[pos++] = i;

    /* Count how many odd indices are of isolated primes */
    size_t needed = 0;
    size_t idx = 1;                  /* prime index starts at 1 */
    for (size_t k = 0; k < prime_cnt; ++k) {
        uint64_t p = prime_list[k];
        bool isolated = false;
        if (p == 2) {
            isolated = true;
        } else {
            bool left  = !prime_flag[p - 2];
            bool right = (p + 2 <= upper) ? !prime_flag[p + 2] : false; /* assume prime if out of range */
            if (left && right) isolated = true;
        }
        if ((idx & 1U) && isolated) ++needed;   /* odd index and isolated */
        ++idx;
    }

    /* Allocate indices array */
    indices = (seq_t*)malloc(needed * sizeof(seq_t));
    if (!indices) return; /* guard */
    idx = 1;
    size_t out = 0;
    for (size_t k = 0; k < prime_cnt; ++k) {
        uint64_t p = prime_list[k];
        bool isolated = false;
        if (p == 2) {
            isolated = true;
        } else {
            bool left  = !prime_flag[p - 2];
            bool right = (p + 2 <= upper) ? !prime_flag[p + 2] : false;
            if (left && right) isolated = true;
        }
        if ((idx & 1U) && isolated) {
            indices[out++] = (seq_t)idx;
        }
        ++idx;
    }
    idx_count = needed;
    current_limit = new_limit;
}

/* Ensure that we have at least `n` terms pre‑computed ------------------------- */
static void ensure_at_least(idx_t n)
{
    if (indices == NULL) {
        recompute_up_to(256);              /* start with a modest limit   */
    }
    while (idx_count < n) {
        size_t new_lim = (current_limit > 0 ? current_limit * 2 : 512);
        recompute_up_to((idx_t)new_lim);
    }
}

/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* offset=1; a(0) is undefined but return 0 */
    ensure_at_least(n);
    return indices[n - 1];
}
