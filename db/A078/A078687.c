/*
 * A078687  Number of x>=0 such that prime(n)-2^x is prime.
 * https://oeis.org/A078687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of x ≥ 0 such that prime(n) – 2^x is prime
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* --- Prime generation utilities ---------------------------------------- */
static uint64_t *prime_buf = NULL;      /* array of primes, increasing capacity */
static size_t   prime_cnt = 0;          /* number of primes stored         */
static size_t   prime_cap = 0;          /* current allocated capacity      */

/* grow the prime buffer to at least new_cap elements */
static void grow_prime_buf(size_t new_cap) {
    if (new_cap <= prime_cap)
        return;
    size_t cap = prime_cap ? prime_cap : 4;
    while (cap < new_cap)
        cap *= 2;
    uint64_t *new_buf = (uint64_t *)realloc(prime_buf, cap * sizeof(uint64_t));
    if (!new_buf) { /* out of memory – abort */
        free(prime_buf);
        prime_buf = NULL;
        prime_cnt = prime_cap = 0;
        return;
    }
    prime_buf = new_buf;
    prime_cap = cap;
}

/* test primality of value v using the stored primes (trial division) */
static bool is_prime_small(uint64_t v) {
    if (v < 2)
        return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_buf[i];
        if (p * p > v)
            break;
        if (v % p == 0)
            return false;
    }
    return true;
}

/* ensure that at least n primes are stored (generate until prime_cnt ≥ n) */
static void ensure_n_primes(idx_t n) {
    if (prime_cnt >= n)
        return;
    uint64_t candidate = prime_cnt ? prime_buf[prime_cnt - 1] + 1 : 2;
    if (candidate < 2)
        candidate = 2;
    /* special case: first prime */
    if (prime_cnt == 0) {
        grow_prime_buf(1);
        prime_buf[0] = 2;
        prime_cnt = 1;
        candidate = 3;
    }
    while (prime_cnt < n) {
        /* skip even numbers */
        if (candidate % 2 == 0)
            ++candidate;
        else {
            if (is_prime_small(candidate)) {
                grow_prime_buf(prime_cnt + 1);
                prime_buf[prime_cnt++] = candidate;
            }
            candidate += 2; /* next odd candidate */
        }
    }
}

/* --- Sequence function ----------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0)          /* below the offset – undefined, return 0 */
        return 0;
    /* make sure we know the n‑th prime */
    ensure_n_primes(n);
    uint64_t p = prime_buf[n - 1];
    seq_t count = 0;
    uint64_t two = 1;
    while (two < p) {            /* 2^x < p  →  p‑2^x ≥ 1   */
        uint64_t diff = p - two;
        if (diff >= 2 && is_prime_small(diff))
            ++count;
        if (two > UINT64_MAX/2)   /* guard against overflow */
            break;
        two <<= 1;
    }
    return count;
}
