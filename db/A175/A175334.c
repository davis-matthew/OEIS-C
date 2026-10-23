/*
 * A175334  a(n) is the smallest positive divisor of Product_{k=1..n} d(k)
 *          that does not yet occur in the sequence, where d(k) is the number
 *          of positive divisors of k.
 * https://oeis.org/A175334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) is the smallest positive divisor of
   Prod_{k=1..n} d(k) that does not yet occur in the sequence,
   where d(k) is the number of positive divisors of k. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Maximum n expected.  Adjust if needed. */
#define MAX_N 10000

/* Primes needed to factor d(k) (all primes <= 127). */
static const int primes[31] = {
    2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,
    53,59,61,67,71,73,79,83,89,97,101,103,107,109,113,127
};
#define PRIME_COUNT 31

/* Static data structures. */
static seq_t *seq = NULL;          /* the sequence values */
static size_t seq_cap = 0;         /* allocated capacity of seq */
static size_t seq_len = 0;         /* number of terms computed */

static uint8_t *visited = NULL;    /* bitset of values already used */
static size_t visited_cap = 0;     /* capacity of visited array */

static uint32_t prod_exp[PRIME_COUNT]; /* exponents of primes in the product */
static uint32_t spf[MAX_N + 1];          /* smallest prime factor array */

/* Ensure seq has at least min_cap elements. */
static void ensure_seq_cap(size_t min_cap)
{
    if (seq_cap >= min_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    if (new_cap < min_cap) new_cap = min_cap;
    seq_t *new_arr = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!new_arr) abort();
    seq = new_arr;
    seq_cap = new_cap;
}

/* Ensure visited bitset can address values up to val. */
static void ensure_vis_cap(idx_t val)
{
    if ((size_t)val <= visited_cap) return;
    size_t new_cap = visited_cap ? visited_cap * 2 : 16;
    if (new_cap < (size_t)val) new_cap = (size_t)val;
    uint8_t *new_arr = (uint8_t *)realloc(visited, new_cap * sizeof(uint8_t));
    if (!new_arr) abort();
    /* initialise new entries to 0 */
    for (size_t i = visited_cap; i < new_cap; ++i) new_arr[i] = 0;
    visited = new_arr;
    visited_cap = new_cap;
}

/* Visited checks. */
static bool is_visited(idx_t val)
{
    if (val == 0) return true;
    if ((size_t)val > visited_cap) return false;
    return visited[val - 1] != 0;
}
static void mark_visited(idx_t val)
{
    ensure_vis_cap(val);
    visited[val - 1] = 1;
}

/* Sieve for smallest prime factor up to MAX_N. */
static void init_spf(void)
{
    for (uint32_t i = 0; i <= MAX_N; ++i) spf[i] = i;
    for (uint32_t i = 2; (uint32_t)(i * i) <= MAX_N; ++i) {
        if (spf[i] == i) {
            for (uint32_t j = i * i; j <= MAX_N; j += i)
                if (spf[j] == j) spf[j] = i;
        }
    }
}

/* Count divisors of k using spf. */
static seq_t divisor_count(idx_t k)
{
    if (k <= 1) return 1;
    seq_t res = 1;
    uint32_t t = (uint32_t)k;
    while (t > 1) {
        uint32_t p = spf[t];
        uint32_t cnt = 0;
        while (t % p == 0) { t /= p; ++cnt; }
        res *= (cnt + 1);
    }
    return res;
}

/* Update prod_exp when multiplying by x (small integer). */
static void factor_into_product(seq_t x)
{
    for (size_t i = 0; i < PRIME_COUNT; ++i) {
        int p = primes[i];
        if ((uint32_t)p * (uint32_t)p > x) break;
        if (x % p == 0) {
            uint32_t e = 0;
            while (x % p == 0) { x /= p; ++e; }
            prod_exp[i] += e;
        }
    }
    if (x > 1) {                    /* remaining prime factor */
        for (size_t i = 0; i < PRIME_COUNT; ++i)
            if (primes[i] == (int)x) { prod_exp[i] += 1; break; }
    }
}

/* Does candidate divide the current product? */
static bool divides_product(idx_t cand)
{
    uint32_t t = (uint32_t)cand;
    for (size_t i = 0; i < PRIME_COUNT; ++i) {
        int p = primes[i];
        if ((uint32_t)p * (uint32_t)p > t) break;
        if (t % p == 0) {
            uint32_t e = 0;
            while (t % p == 0) { t /= p; ++e; }
            if (e > prod_exp[i]) return false;
        }
    }
    if (t > 1) {                    /* remaining prime factor */
        for (size_t i = 0; i < PRIME_COUNT; ++i)
            if (primes[i] == (int)t) { return prod_exp[i] > 0; }
        return false;
    }
    return true;
}

/* One-time initialization. */
static void init_once(void)
{
    static bool ready = false;
    if (ready) return;
    ready = true;
    for (size_t i = 0; i < PRIME_COUNT; ++i) prod_exp[i] = 0;
    ensure_seq_cap(0);
    visited_cap = 0;
    visited = NULL;
    init_spf();
}

/* -------------------------------------------------------------- */
/* Main function required by the build. */
seq_t a(idx_t n)
{
    init_once();

    if (n == 0) return 0;   /* undefined for n=0 */

    /* generate terms until we reach n */
    while (seq_len < (size_t)n) {
        idx_t k = seq_len + 1;           /* next index to process */

        /* multiply in d(k) into product */
        seq_t dk = divisor_count(k);
        factor_into_product(dk);

        /* find smallest positive divisor of product not yet used */
        idx_t cand = 1;
        while (true) {
            if (!is_visited(cand) && divides_product(cand)) break;
            ++cand;
        }

        /* record the new term */
        ensure_seq_cap(seq_len + 1);
        seq[seq_len] = (seq_t)cand;
        ++seq_len;
        mark_visited(cand);
    }

    return seq[n - 1];
}
