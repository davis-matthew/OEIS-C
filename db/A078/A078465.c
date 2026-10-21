/*
 * A078465  Primonacci numbers:
 *          a(n)=a(n-2)+a(n-3)+a(n-5)+a(n-7)+a(n-11)+...+a(n-p(k))+... until n
 *          <= p(k), where p(k) is the k-th prime. a(1)=a(2)=1.
 * https://oeis.org/A078465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 31, u32 n <= 59, u64 n <= 116,
 *            u128 n <= 230, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=a(2)=1; a(n)=Σ a(n‑p) over all primes p≤n (n≥3) */
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Memoized sequence values */
static seq_t *seq_arr = NULL;
static size_t seq_cap = 0;   /* allocated capacity */
static idx_t  seq_max = 0;   /* highest index computed */

/* Prime sieve data */
static uint64_t *primes = NULL;  /* list of primes up to last sieve limit */
static size_t   primes_cnt = 0;  /* number of primes stored */
static idx_t    primes_cap = 0;  /* current prime sieve limit */

/* Sieve primes up to at least 'limit' using a simple Eratosthenes algorithm.
   The primes list is rebuilt from scratch each time a larger limit is
   requested. For n ≤ 10 000 this is trivial. */
static void ensure_primes(idx_t limit)
{
    if (limit <= primes_cap) return;

    /* Allocate enough space for the prime list */
    size_t new_cap = (size_t)limit + 1;
    uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    primes = tmp;

    /* Simple sieve of Eratosthenes up to 'limit' */
    bool *is_comp = (bool *)calloc(new_cap, sizeof(bool));
    if (!is_comp) abort();

    primes_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            primes[primes_cnt++] = i;
            if ((idx_t)i * (idx_t)i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i) {
                    is_comp[j] = true;
                }
            }
        }
    }
    free(is_comp);
    primes_cap = limit;
}

/* Ensure that seq_arr contains values up to index 'n'. */
static void ensure_seq(idx_t n)
{
    if (n <= seq_max) return;

    /* Grow array as necessary */
    if (seq_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq_arr = tmp;
        seq_cap = new_cap;
    }

    /* Compute missing terms */
    for (idx_t i = seq_max + 1; i <= n; ++i) {
        if (i == 1 || i == 2) {
            seq_arr[i] = 1;
        } else {
            ensure_primes(i);
            seq_t sum = 0;
            for (size_t j = 0; j < primes_cnt; ++j) {
                uint64_t p = primes[j];
                if (p > i) break;
                sum += seq_arr[i - p];
            }
            seq_arr[i] = sum;
        }
    }
    seq_max = n;
}

/* Public entry point defined by the specification. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* safety for out-of-range */
    ensure_seq(n);
    return seq_arr[n];
}
