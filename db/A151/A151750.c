/*
 * A151750  Numbers n such that gcd( binomial(2n,n), 3*5*7*11 ) = 1.
 * https://oeis.org/A151750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, bref
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: generate the numbers n such that gcd(binomial(2n,n),1155)=1 via brute‑force scan. */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_candidate = 0;

/* Ensure the cache has enough capacity. */
static void add_term(seq_t val) {
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Out of memory: abort. */
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Compute the sum of the digits of n in the given base. */
static seq_t digit_sum(seq_t n, int base) {
    seq_t sum = 0;
    while (n > 0) {
        sum += n % base;
        n /= base;
    }
    return sum;
}

/* Return true if n is a valid term: no carries when n+n is added in any prime base. */
static bool valid_term(seq_t n) {
    const int primes[4] = {3, 5, 7, 11};
    for (int i = 0; i < 4; ++i) {
        int p = primes[i];
        seq_t sum_n = digit_sum(n, p);
        seq_t n2 = n * 2;
        seq_t sum_2n = digit_sum(n2, p);
        /* A carry occurs if sum_2n < 2*sum_n.  */
        if (sum_2n < sum_n * 2)
            return false;   /* Divisible by this prime. */
    }
    return true;           /* No prime causes a carry. */
}

/* Return the nth term of the sequence (1‑based index). */
seq_t a(idx_t n) {
    if (n == 0) {
        /* The sequence is defined for n >= 1.  */
        abort();
    }
    while (cache_len < n) {
        seq_t cand = (seq_t)next_candidate;
        if (valid_term(cand))
            add_term(cand);
        next_candidate++;
    }
    return cache[n - 1];
}
