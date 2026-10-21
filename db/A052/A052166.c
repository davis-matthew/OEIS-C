/*
 * A052166  Primes at which the difference pattern X424Y (X and Y >= 6) occurs
 *          in A001223.
 * https://oeis.org/A052166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   ensure_seq_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A052166: Primes p for which the consecutive gaps
   {p - previous, 4, 2, 4, next - p} satisfy X ≥ 6, 4, 2, 4, Y ≥ 6.
   The code generates primes sequentially and stores the central prime
   of each matching 6‑prime block.  Results are memoised and can be
   accessed in any order of calls.
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ensure_seq_capacity.h"

static seq_t *results = NULL;
static size_t results_len = 0, results_cap = 0;

static void push_result(seq_t val)
{
    ensure_seq_capacity(&results, &results_len, &results_cap, results_len + 1);
    results[results_len++] = val;
}

/* sliding window of the last 6 primes */
static seq_t window[6];
static int window_size = 0;

/* small primes for sieving */
#define BASE_PRIME_LIMIT 1000000U
static uint64_t *base_primes = NULL;
static size_t base_primes_cnt = 0;

static void init_base_primes(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    size_t limit = BASE_PRIME_LIMIT;
    bool *is_prime = (bool *)malloc(limit + 1);
    if (!is_prime) return;
    memset(is_prime, 1, limit + 1);
    is_prime[0] = is_prime[1] = 0;
    for (size_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;

    base_primes_cnt = 0;
    base_primes = (uint64_t *)malloc(limit * sizeof(uint64_t));
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) base_primes[base_primes_cnt++] = i;
    free(is_prime);
}

/* generate results up to index n (1‑based) */
static void generate_to(idx_t n)
{
    if (n <= results_len) return;
    init_base_primes();

    const uint64_t SEGMENT_SIZE = 1 << 20;   /* 1,048,576 */
    static uint64_t low = 2, high;
    if (high == 0) high = low + SEGMENT_SIZE - 1;

    while (results_len < (size_t)n) {
        size_t sz = (size_t)(high - low + 1);
        bool *is_prime = (bool *)malloc(sz);
        if (!is_prime) return;
        memset(is_prime, 1, sz);
        if (low == 1) is_prime[0] = 0;   /* 1 is not prime */

        for (size_t i = 0; i < base_primes_cnt; ++i) {
            uint64_t p = base_primes[i];
            uint64_t p_sq = p * p;
            if (p_sq > high) break;
            uint64_t start = (low + p - 1) / p * p;
            if (start < p_sq) start = p_sq;
            for (uint64_t j = start; j <= high; j += p)
                is_prime[j - low] = 0;
        }

        for (size_t i = 0; i < sz; ++i) {
            if (!is_prime[i]) continue;
            uint64_t val = low + i;
            seq_t p = (seq_t)val;

            if (window_size < 6) {
                window[window_size++] = p;
            } else {
                for (int k = 0; k < 5; ++k) window[k] = window[k + 1];
                window[5] = p;
            }

            if (window_size == 6) {
                seq_t d1 = window[1] - window[0];
                seq_t d2 = window[2] - window[1];
                seq_t d3 = window[3] - window[2];
                seq_t d4 = window[4] - window[3];
                seq_t d5 = window[5] - window[4];
                if (d1 >= (seq_t)6 &&
                    d2 == (seq_t)4 &&
                    d3 == (seq_t)2 &&
                    d4 == (seq_t)4 &&
                    d5 >= (seq_t)6)
                    push_result(window[1]);   /* central prime */
            }
        }

        free(is_prime);
        low = high + 1;
        high += SEGMENT_SIZE;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                        /* undefined for n<1 */
    generate_to(n);
    return results[n - 1];
}
