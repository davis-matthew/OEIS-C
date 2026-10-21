/*
 * A076821  Squares of the differences between consecutive primes.
 * https://oeis.org/A076821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (p_{n+1} - p_n)^2 (square of the difference between
 *            consecutive primes; a(1) = (3-2)^2 = 1)
 * Range:     u8 n <= 98, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Static storage for primes generated incrementally. */
static idx_t *prime_list = NULL;      /* 0‑based array of primes          */
static size_t prime_cap = 0;          /* allocated capacity of prime_list */
static size_t prime_cnt = 0;          /* number of primes stored          */
static idx_t  prime_next = 2;         /* next candidate to test          */

/* Ensure at least 'need' primes are present in prime_list. */
static void ensure_primes(size_t need)
{
    if (prime_cnt >= need)
        return;

    /* Grow prime_list if required. */
    if (prime_cap < need) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 16;
        if (new_cap < need)
            new_cap = need;
        idx_t *tmp = (idx_t *)realloc(prime_list, new_cap * sizeof(idx_t));
        if (!tmp)
            return; /* allocation failed – keep state unchanged */
        prime_list = tmp;
        prime_cap = new_cap;
    }

    /* Append primes until we have enough. */
    while (prime_cnt < need) {
        bool isprime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            idx_t p = prime_list[i];
            if ((idx_t)p * p > prime_next)
                break;
            if (prime_next % p == 0) {
                isprime = false;
                break;
            }
        }
        if (isprime)
            prime_list[prime_cnt++] = prime_next;
        ++prime_next;
    }
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;

    /* Need (n+1)-th prime: indices are 0‑based. */
    ensure_primes((size_t)n + 1);

    idx_t p_prev = prime_list[n - 1];      /* p_n   */
    idx_t p_curr = prime_list[n];          /* p_{n+1} */
    idx_t diff = p_curr - p_prev;          /* positive difference */

    seq_t d = (seq_t)diff;                 /* Promote to seq_t */
    return d * d;                          /* Square */
}
