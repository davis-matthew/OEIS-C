/*
 * A142900  Primes congruent to 20 mod 63.
 * https://oeis.org/A142900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime ≡ 20 (mod 63) (offset = 1)
 * Range:     u8 n <= 1, u16 n <= 176, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* dynamic containers for all primes and for those ≡ 20 (mod 63) */
static uint64_t *prime_list   = NULL;   /* all primes found */
static size_t   prime_cnt    = 0;
static size_t   prime_cap    = 0;

static uint64_t *special_list = NULL;   /* primes ≡ 20 (mod 63) */
static size_t   special_cnt   = 0;
static size_t   special_cap   = 0;

/* integer square root of an unsigned 64‑bit integer (no overflow) */
static uint64_t isqrt_u64(uint64_t n)
{
    if (n < 2)
        return n;
    uint64_t low  = 1;
    uint64_t high = n >> 1;
    uint64_t mid, res = 1;

    while (low <= high) {
        mid = low + ((high - low) >> 1);
        if (mid <= n / mid) {
            res = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return res;
}

/* helpers to grow the dynamic arrays */
static void add_prime(uint64_t p)
{
    if (prime_cnt == prime_cap) {
        prime_cap = prime_cap ? prime_cap * 2 : 16;
        uint64_t *newp = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
        if (!newp) exit(1);
        prime_list = newp;
    }
    prime_list[prime_cnt++] = p;
}

static void add_special(uint64_t p)
{
    if (special_cnt == special_cap) {
        special_cap = special_cap ? special_cap * 2 : 8;
        uint64_t *newp = (uint64_t *)realloc(special_list, special_cap * sizeof(uint64_t));
        if (!newp) exit(1);
        special_list = newp;
    }
    special_list[special_cnt++] = p;
}

/* extend the sieve until we have at least n special primes */
static void ensure_special_primes(idx_t n)
{
    static uint64_t last_checked = 1;

    while (special_cnt < n) {
        /* increase the search range stepwise */
        uint64_t next_limit = last_checked ? last_checked * 2 : 100;
        if (next_limit < last_checked) next_limit = ~0ULL; /* safeguard overflow */

        for (; last_checked < next_limit; ++last_checked) {
            if (last_checked < 2)
                continue;

            bool is_prime = true;
            uint64_t limit = isqrt_u64(last_checked);
            for (size_t i = 0; i < prime_cnt && prime_list[i] <= limit; ++i) {
                if (last_checked % prime_list[i] == 0) {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime) {
                add_prime(last_checked);
                if ((last_checked % 63) == 20)
                    add_special(last_checked);
            }
        }
    }
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n<1, but return 0 safely */

    ensure_special_primes(n);
    return (seq_t)special_list[n - 1];
}
