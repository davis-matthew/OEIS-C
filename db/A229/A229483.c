/*
 * A229483  Squarefree numbers whose indices are lucky numbers.
 * https://oeis.org/A229483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A005117(A000959(n)) Compute the nth square‑free number
 *            whose index is the nth lucky number
 * Range:     u8 n <= 32, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   compute_lucky_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/compute_lucky_n.h"

/* ---------- auxiliary data for prime sieve ---------- */
#define MAX_PRIMES 2000

static uint64_t prime_list[MAX_PRIMES];
static size_t   prime_cnt = 0;
static uint64_t prime_limit = 0;

/* generate all primes ≤ limit (simple sieve) */
static void build_primes_upto(uint64_t limit)
{
    if (prime_limit >= limit)
        return;

    size_t sz = (size_t)limit + 1;
    uint8_t *is_prime = (uint8_t*)malloc(sz);
    if (!is_prime)
        return;
    for (size_t i = 0; i < sz; ++i)
        is_prime[i] = 1;
    if (sz > 0) is_prime[0] = 0;
    if (sz > 1) is_prime[1] = 0;

    for (size_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;

    prime_cnt = 0;
    for (size_t i = 2; i <= limit && prime_cnt < MAX_PRIMES; ++i)
        if (is_prime[i])
            prime_list[prime_cnt++] = (uint64_t)i;

    free(is_prime);
    prime_limit = limit;
}

/* test if a number is square‑free using the prime list */
static bool is_squarefree_val(uint64_t m)
{
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p  = prime_list[i];
        uint64_t p2 = p * p;
        if (p2 > m)
            break;
        if ((m % p2) == 0)
            return false;
    }
    return true;
}

/* ---------- lucky numbers ---------- */

/* ---------- square‑free numbers ---------- */

/* compute the k‑th square‑free number (1‑based) */
static seq_t compute_squarefree_n(idx_t k)
{
    if (k < 1) return (seq_t)0;

    /* ensure we have primes up to sqrt of likely max number */
    build_primes_upto(1000);          /* 1000 > sqrt(1e6) – enough for n≤1e5 */

    size_t count = 0;
    for (uint64_t m = 1; ; ++m) {
        if (is_squarefree_val(m)) {
            ++count;
            if (count == (size_t)k)
                return (seq_t)m;
        }
    }
}

/* ---------- final sequence function ---------- */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    seq_t lucky_val = compute_lucky_n(n);      /* A000959(n) */
    idx_t sf_index  = (idx_t)lucky_val;        /* index for square‑free list */
    return compute_squarefree_n(sf_index);     /* A005117(sf_index) */
}
