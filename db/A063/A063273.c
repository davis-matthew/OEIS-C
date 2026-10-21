/*
 * A063273  Number of times most common digit of primes appears in first n
 *          primes.
 * https://oeis.org/A063273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of occurrences of the most frequent decimal digit
 *            among the first n primes. (Offset 1)
 * Range:     u8 n <= 292, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static idx_t     next_candidate = 2;          /* next number to test for primality   */
static idx_t    *prime_list      = NULL;      /* dynamic array of primes             */
static size_t    prime_cnt       = 0;          /* number of primes found              */
static size_t    prime_cap       = 0;          /* allocated capacity of prime_list    */

static seq_t    *max_counts      = NULL;      /* max digit counts for prefixes      */
static size_t    max_cnt         = 0;          /* stored max up to current n          */
static size_t    max_cap         = 0;          /* allocated capacity of max_counts    */

static seq_t digit_counts[10] = {0};           /* cumulative digit frequencies       */

/* Grow dynamic array to at least 'need' elements, using realloc. */
static void ensure_capacity(void **ptr, size_t *cap, size_t need, size_t elem_size)
{
    if (need <= *cap) return;
    size_t new_cap = (*cap == 0) ? 16 : *cap;
    while (new_cap < need) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) { abort(); }                      /* out of memory                */
    *ptr = tmp;
    *cap = new_cap;
}

/* Test whether x is prime by trial division using the primes found so far. */
static bool is_candidate_prime(idx_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_list[i];
        if ((idx_t)(p * p) > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* Incrementally generate primes up to target_n, updating digit_counts and max_counts. */
static void generate_up_to(idx_t target_n)
{
    while (prime_cnt < target_n) {
        /* find next prime */
        while (!is_candidate_prime(next_candidate)) ++next_candidate;

        /* store prime */
        ensure_capacity((void**)&prime_list, &prime_cap, prime_cnt + 1, sizeof(idx_t));
        prime_list[prime_cnt] = next_candidate;

        /* update digit frequencies */
        idx_t t = next_candidate;
        while (t > 0) {
            unsigned d = (unsigned)(t % 10);
            digit_counts[d] += 1;
            t /= 10;
        }

        /* compute current maximum digit frequency */
        seq_t max_f = 0;
        for (int d = 0; d < 10; ++d)
            if (digit_counts[d] > max_f) max_f = digit_counts[d];

        /* store answer for this prefix */
        ensure_capacity((void**)&max_counts, &max_cap, prime_cnt + 1, sizeof(seq_t));
        max_counts[prime_cnt] = max_f;

        /* prepare for next candidate */
        ++next_candidate;
        ++prime_cnt;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined offset, but safeguard   */
    if (n > max_cnt) generate_up_to(n);
    return max_counts[n-1];
}
