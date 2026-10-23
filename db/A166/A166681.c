/*
 * A166681  Primes p which have at least one prime anagram larger than p.
 * https://oeis.org/A166681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 791, u32 n <= 791, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A166681: primes p such that at least one anagram of p is a prime larger than p. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"   /* is_prime_big(seq_t) */
#include "../utils/next_prime.h"     /* next_prime(uint64_t) */

static seq_t *terms = NULL;          /* array of found primes */
static size_t terms_size = 0;
static size_t terms_cap  = 0;
static uint64_t next_candidate = 2;  /* next number to test for primality */
static seq_t pow10[11];              /* pow10[i] = 10^i */

/* Precompute 10^i */
static void init_pow10(void) {
    pow10[0] = (seq_t)1;
    for (int i = 1; i <= 10; ++i) {
        pow10[i] = pow10[i-1] * (seq_t)10;
    }
}

/* Recursive search for a larger prime anagram */
static bool has_larger_prime_anagram_rec(int cnt[10], int depth, int len,
                                         seq_t cur, const seq_t p) {
    if (depth == len) {
        if (cur > p && is_prime_big(cur)) return true;
        return false;
    }
    int remaining = len - depth - 1;
    for (int d = 0; d < 10; ++d) {
        if (cnt[d] == 0) continue;
        if (depth == 0 && d == 0) continue;    /* avoid leading zero */
        cnt[d]--;
        seq_t next = cur * (seq_t)10 + (seq_t)d;
        /* prune branches whose maximum possible number <= p */
        seq_t max_possible = next * pow10[remaining] + (pow10[remaining] - (seq_t)1);
        if (max_possible <= p) {
            cnt[d]++;
            continue;
        }
        if (has_larger_prime_anagram_rec(cnt, depth + 1, len, next, p)) {
            cnt[d]++;
            return true;
        }
        cnt[d]++;
    }
    return false;
}

/* Check if prime p has a larger prime anagram */
static bool has_larger_prime_anagram(uint64_t p) {
    int cnt[10] = {0};
    int len = 0;
    uint64_t t = p;
    if (t == 0) { cnt[0] = 1; len = 1; }
    while (t > 0) {
        int d = t % 10;
        cnt[d]++;
        len++;
        t /= 10;
    }
    return has_larger_prime_anagram_rec(cnt, 0, len, (seq_t)0, (seq_t)p);
}

/* Ensures the static array has at least n elements */
static void ensure_n(size_t n) {
    while (terms_size < n) {
        uint64_t p = next_prime(next_candidate);
        next_candidate = p + 1;
        if (has_larger_prime_anagram(p)) {
            if (terms_size == terms_cap) {
                size_t new_cap = (terms_cap == 0) ? 16 : terms_cap * 2;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE); /* allocation failed */
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_size++] = (seq_t)p;
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* undefined for n < 1, but safe guard */
    static bool initialized = false;
    if (!initialized) {
        init_pow10();
        initialized = true;
    }
    ensure_n((size_t)n);
    return terms[(size_t)n - 1];
}
