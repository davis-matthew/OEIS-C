/*
 * A115732  n-th prime prime(n) repeated (prime(n)-n) times.
 * https://oeis.org/A115732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = prime(n) repeated (prime(n)-n) times
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* seq_t and idx_t are provided by the build. */
 
/* static storage for primes and cumulative block sizes */
static uint32_t *prime_arr   = NULL;
static size_t   prime_cap   = 0;
static size_t   prime_cnt   = 0;

static idx_t   *cum_arr    = NULL;
static size_t   cum_cap    = 0;
static size_t   cum_cnt    = 0;

/* ensure capacity of a dynamic array of uint32_t */
static void ensure_prime_cap(size_t needed)
{
    if (prime_cap < needed) {
        size_t newcap = prime_cap == 0 ? 8 : prime_cap << 1;
        while (newcap < needed) newcap <<= 1;
        uint32_t *tmp = (uint32_t *)realloc(prime_arr, newcap * sizeof(uint32_t));
        prime_arr = tmp;
        prime_cap = newcap;
    }
}

/* ensure capacity of a dynamic array of idx_t */
static void ensure_cum_cap(size_t needed)
{
    if (cum_cap < needed) {
        size_t newcap = cum_cap == 0 ? 8 : cum_cap << 1;
        while (newcap < needed) newcap <<= 1;
        idx_t *tmp = (idx_t *)realloc(cum_arr, newcap * sizeof(idx_t));
        cum_arr = tmp;
        cum_cap = newcap;
    }
}

/* check primality of candidate using the primes seen so far */
static bool is_candidate_prime(uint32_t candidate, size_t upto)
{
    for (size_t i = 0; i < upto; ++i) {
        uint32_t p = prime_arr[i];
        uint64_t sq = (uint64_t)p * (uint64_t)p;
        if (sq > candidate) break;
        if (candidate % p == 0) return false;
    }
    return true;
}

/* function producing nth term of A115732 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* out of defined range, but no harm */

    /* generate primes and cumulative sums until we cover n */
    while (cum_cnt == 0 || cum_arr[cum_cnt - 1] < n) {

        uint32_t next_prime;

        if (prime_cnt == 0) {
            next_prime = 2;
        } else {
            /* start searching from the next integer after last prime */
            uint32_t candidate = prime_arr[prime_cnt - 1] + 1;
            /* skip even numbers after 2 */
            if (candidate % 2 == 0) ++candidate;
            for (;;) {
                if (is_candidate_prime(candidate, prime_cnt)) {
                    next_prime = candidate;
                    break;
                }
                candidate += 2;
            }
        }

        /* store the new prime */
        ensure_prime_cap(prime_cnt + 1);
        prime_arr[prime_cnt] = next_prime;
        prime_cnt++;

        /* compute block length and cumulative sum */
        idx_t block_len = (idx_t)next_prime - (idx_t)prime_cnt;
        idx_t cum_before = (cum_cnt == 0) ? 0 : cum_arr[cum_cnt - 1];
        idx_t new_cum = cum_before + block_len;

        /* store cumulative */
        ensure_cum_cap(cum_cnt + 1);
        cum_arr[cum_cnt] = new_cum;
        cum_cnt++;
    }

    /* binary search for the first cumulative sum >= n */
    size_t left = 0, right = cum_cnt - 1;
    while (left < right) {
        size_t mid = left + (right - left) / 2;
        if (cum_arr[mid] >= n) {
            right = mid;
        } else {
            left = mid + 1;
        }
    }

    /* left is the index of the prime that contains position n */
    seq_t result = (seq_t)prime_arr[left];
    return result;
}
