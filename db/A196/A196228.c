/*
 * A196228  Number of ways of writing n as sum of a prime and a perfect power.
 * https://oeis.org/A196228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of sequence A196228: a(n) = #{(p,k) | p prime, k perfect power ≥1, p + k = n} */

/* Standard headers – the build supplies seq_t and idx_t types. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ---------- Static state for memoised sieves ---------- */
static uint8_t *prime_flag   = NULL;   /* flag array up to current max_prime_limit   */
static uint8_t *pp_flag      = NULL;   /* perfect‑power flag array up to max_pp_limit */
static uint64_t *prime_list  = NULL;   /* list of primes up to current max_prime_limit */
static size_t   prime_count  = 0;      /* number of primes stored */
static size_t   prime_cap    = 0;      /* allocated capacity of prime_list          */
static idx_t    max_prime_limit = 0;   /* current limit of prime sieve            */
static idx_t    max_pp_limit    = 0;   /* current limit of perfect‑power sieve    */

/* Extend capacity of prime_list if needed. */
static void grow_prime_capacity(size_t needed)
{
    if (prime_cap >= needed) return;
    size_t new_cap = prime_cap ? prime_cap : 16;
    while (new_cap < needed) new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    prime_list = tmp;
    prime_cap  = new_cap;
}

/* Recompute the prime sieve up to 'limit'.  If the limit is not larger than
 * the current one, nothing is done.  The prime list is rebuilt each time.
 */
static void update_prime_sieve(idx_t limit)
{
    if (limit <= max_prime_limit) return;

    /* Allocate new flag array and run the sieve. */
    uint8_t *new_flag = (uint8_t *)calloc((size_t)limit + 1, sizeof(uint8_t));
    if (!new_flag) abort();                         /* allocation failure */
    for (idx_t i = 2; i <= limit; ++i) new_flag[i] = 1;
    for (idx_t i = 2; (i * i) <= limit; ++i) {
        if (new_flag[i]) {
            for (idx_t j = i * i; j <= limit; j += i) new_flag[j] = 0;
        }
    }

    /* Replace old flag array. */
    free(prime_flag);
    prime_flag = new_flag;
    max_prime_limit = limit;

    /* Build the prime list from the flag array. */
    size_t count = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (prime_flag[i]) ++count;

    grow_prime_capacity(count);
    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (prime_flag[i]) prime_list[pos++] = (uint64_t)i;

    prime_count = count;
}

/* Recompute the perfect‑power sieve up to 'limit'.  Perfect powers include
 * 1 and all k = base^exp with base ≥ 2 and exp ≥ 2.  Only the unique values
 * are stored in the flag array.
 */
static void update_pp_sieve(idx_t limit)
{
    if (limit <= max_pp_limit) return;

    uint8_t *new_flag = (uint8_t *)calloc((size_t)limit + 1, sizeof(uint8_t));
    if (!new_flag) abort();

    if (limit >= 1) new_flag[1] = 1;          /* 1 is a perfect power (1^k) */
    for (idx_t base = 2; base <= limit; ++base) {
        uint64_t val = (uint64_t)base * (uint64_t)base;  /* base² */
        while (val <= (uint64_t)limit) {
            new_flag[(size_t)val] = 1;
            if (val > (uint64_t)limit / base) break; /* would overflow */
            val *= base;
        }
    }

    free(pp_flag);
    pp_flag = new_flag;
    max_pp_limit = limit;
}

/* Ensure that sieves cover all numbers up to 'limit'. */
static void ensure_prime_and_perfect_powers(idx_t limit)
{
    /* We need primes up to limit‑1 because p ≤ n‑1. */
    update_prime_sieve(limit > 1 ? limit - 1 : limit);
    /* Perfect powers up to limit itself. */
    update_pp_sieve(limit);
}

/* Main sequence function.  It works for all seq_t and idx_t types
 * supported by the build, including arbitrary‑precision seq_t.
 */
seq_t a(idx_t n)
{
    if (n < 1) return 0;       /* offsets below 1 are irrelevant */

    ensure_prime_and_perfect_powers(n);

    seq_t count = 0;
    /* Iterate over all primes p ≤ n‑1. */
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p > (uint64_t)(n - 1)) break;

        idx_t rem = n - (idx_t)p;
        if (rem >= 1 && pp_flag[rem]) {
            ++count;
        }
    }
    return count;
}
