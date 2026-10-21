/*
 * A068828  Geometrically weak primes: primes that are smaller than the
 *          geometric mean of their neighbors (2 is included by convention).
 * https://oeis.org/A068828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 3137, u32 n <= 4774, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A068828 : Geometrically weak primes.
 *  p_k^2 <= p_{k-1} * p_{k+1}  (2 is included by convention).
 *  The routine maintains a dynamic list of primes and a list of
 *  geometrically weak primes;  it produces a(n) for any n ≥ 1 in
 *  approximately O(n) time.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* static tables of primes and weak primes, expanded on demand */
static uint64_t *prime_list = NULL; /* stored as 64‑bit values */
static size_t        prime_cnt = 0;
static size_t        prime_cap = 0;

static seq_t *weak_list = NULL;
static size_t weak_cnt = 0;
static size_t weak_cap = 0;

/* last integer that has been examined for primality */
static uint64_t last_checked = 1;

/* ensure space for a new prime, then add it */
static void add_prime(uint64_t p)
{
    if (prime_cnt == prime_cap) {
        size_t new_cap = prime_cap ? prime_cap << 1 : 64;
        uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
        if (!tmp) exit(EXIT_FAILURE);
        prime_list = tmp;
        prime_cap = new_cap;
    }
    prime_list[prime_cnt++] = p;
}

/* ensure space for a new weak prime, then add it */
static void add_weak(seq_t p)
{
    if (weak_cnt == weak_cap) {
        size_t new_cap = weak_cap ? weak_cap << 1 : 8;
        seq_t *tmp = (seq_t *)realloc(weak_list, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        weak_list = tmp;
        weak_cap = new_cap;
    }
    weak_list[weak_cnt++] = p;
}

/* is_sequence_p_k we test while adding a new prime, the middle prime
 * (previous prime) becomes the candidate for a weak prime.
 * The check is: p_prev^2 <= p_prevPrev * p_curr
 * All values are promoted to seq_t for arbitrary‑precision builds. */
static void try_add_weak(void)
{
    if (prime_cnt < 3) return;  /* need three consecutive primes */

    seq_t prev_prev = (seq_t)prime_list[prime_cnt - 3];
    seq_t prev      = (seq_t)prime_list[prime_cnt - 2];
    seq_t curr      = (seq_t)prime_list[prime_cnt - 1];

    /* if prev^2 <= prev_prev * curr, prev is weak */
    if (prev * prev <= prev_prev * curr) {
        add_weak(prev);
    }
}

/* prime test: simple trial division using known primes */
static bool is_prime(uint64_t cand)
{
    if (cand < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > cand) break;
        if (cand % p == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* initialise only once */
    if (weak_cnt == 0) {
        add_prime(2);          /* first prime */
        add_weak(2);           /* 2 is included by convention */
        last_checked = 2;
    }

    /* generate primes until we have at least n weak primes */
    while (weak_cnt < (size_t)n) {
        uint64_t candidate = last_checked + 1;
        if (is_prime(candidate)) {
            add_prime(candidate);
            try_add_weak();
        }
        last_checked = candidate;
    }

    return weak_list[n - 1];
}
