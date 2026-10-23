/*
 * A107741  Smallest number m such that prime(n) = m + (digit sum of m),
 *          a(n)=0 if no such m exists.
 * https://oeis.org/A107741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 6547, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A107741 :  a(n) = smallest m such that prime(n) = m + digit_sum(m);
 *  returns 0 if no such m exists.
 *  The implementation uses a dynamic sieve of Eratosthenes to obtain
 *  primes.  It works for any integer type seq_t (including arbitrary‑precision)
 *  and for idx_t = uint64_t.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <stddef.h>
#include "../utils/digit_sum.h"   /* provides seq_t digit_sum(seq_t) */

/* ----------------------------------------------------------------------- */
/*  Dynamic sieve of Eratosthenes                                            */
/* ----------------------------------------------------------------------- */

static uint64_t *prime_list = NULL;   /* 1‑based array of primes */
static size_t   prime_cnt  = 0;       /* number of primes stored          */
static size_t   sieve_lim  = 0;       /* current upper bound (inclusive) */

/* Run a classic sieve up to 'limit' (inclusive). */
static void sieve_up_to(size_t limit)
{
    if (limit < 2) {
        prime_cnt = 0;
        sieve_lim = limit;
        return;
    }

    uint8_t *is_comp = (uint8_t*)malloc(limit + 1);
    if (!is_comp) abort();
    memset(is_comp, 0, limit + 1);
    is_comp[0] = 1;
    is_comp[1] = 1;

    size_t root = (size_t) sqrt((double)limit);
    for (size_t p = 2; p <= root; ++p) {
        if (!is_comp[p]) {
            for (size_t q = p * p; q <= limit; q += p)
                is_comp[q] = 1;
        }
    }

    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) ++cnt;

    uint64_t *new_list = (uint64_t*)realloc(prime_list, cnt * sizeof(uint64_t));
    if (!new_list) abort();
    prime_list = new_list;

    size_t pos = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) prime_list[pos++] = (uint64_t)i;

    prime_cnt = cnt;
    sieve_lim = limit;
    free(is_comp);
}

/* Return the n‑th prime (1‑based); enlarge the sieve if necessary. */
static uint64_t nth_prime(idx_t n)
{
    if (n == 0) return 0;
    if ((size_t)n <= prime_cnt) return prime_list[n - 1];

    /* Heuristic upper bound for the n‑th prime. */
    size_t estimate;
    if (n < 6) {
        estimate = 15;
    } else {
        double nd = (double)n;
        estimate = (size_t)(nd * (log(nd) + log(log(nd))) + 10.0);
    }
    if (estimate <= sieve_lim) estimate = sieve_lim * 2 + 100;

    sieve_up_to(estimate);

    /* If the sieve is still too small, double it until we have enough primes */
    while (prime_cnt < (size_t)n) {
        size_t new_lim = sieve_lim * 2 + 100;
        sieve_up_to(new_lim);
    }
    return prime_list[n - 1];
}

/* ----------------------------------------------------------------------- */
/*  A107741 sequence                                                      */
/* ----------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* The OEIS offset is 1. For n==0 we return 0 (undefined). */
    if (n == 0) return (seq_t)0;

    uint64_t p = nth_prime(n);            /* the n‑th prime */

    /* Count decimal digits of p */
    uint64_t tmp  = p;
    uint64_t digits = 0;
    do { ++digits; tmp /= 10; } while (tmp);

    uint64_t max_digit_sum = 9 * digits;
    if (max_digit_sum == 0) return (seq_t)0;

    seq_t best_m = 0;

    /* Search for the smallest m: try larger digit sums first. */
    for (uint64_t d = max_digit_sum; d >= 1; --d) {
        if (p <= d) continue;            /* would give m <= 0 */
        uint64_t m = p - d;
        if (m == 0) continue;
        if (digit_sum((seq_t)m) == (seq_t)d) {
            best_m = (seq_t)m;
            break;                       /* first found is the smallest m */
        }
    }
    return best_m;    /* 0 if none found */
}
