/*
 * A107187  Primes of the form 7x^2 + 11y^2.
 * https://oeis.org/A107187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 416, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A107187: Primes of the form 7x² + 11y².
   The function tests primes for representability by direct search. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Prototype from the test harness */
#include "../utils/is_prime_uint64.h"

static seq_t *prime_list = NULL;   /* stored primes */
static size_t   prime_cnt  = 0;     /* number of primes stored */
static size_t   prime_cap  = 0;     /* capacity of prime_list */
static uint64_t next_start = 2;     /* next candidate to test */

/* Integer square root using binary search (fits in C11/C++17). */
static uint64_t
isqrt_uint64(uint64_t n)
{
    uint64_t low = 0, high = 0xFFFFFFFFULL + 1ULL; /* sqrt(2^64-1) < 2^32 */
    while (low < high) {
        uint64_t mid = (low + high) >> 1;
        if (mid == 0) { low = mid + 1; continue; }
        if (((unsigned __int128)mid) * mid < n) {
            low = mid + 1;
        } else if (((unsigned __int128)mid) * mid > n) {
            high = mid;
        } else {
            return mid;
        }
    }
    return low - 1;
}

/* Check if n is a perfect square. */
static bool
is_square_uint64(uint64_t n)
{
    uint64_t r = isqrt_uint64(n);
    return r * r == n;
}

/* Does the prime p represent 7x² + 11y²? */
static bool
representablePrime(uint64_t p)
{
    if (p == 7 || p == 11) return true;

    /* Search for integer y with 11*y² <= p. */
    uint64_t y_max = isqrt_uint64(p / 11);
    for (uint64_t y = 0; y <= y_max; ++y) {
        uint64_t yy = y * y;
        uint64_t prod = 11 * yy;
        if (prod > p) break;                 /* safety, though loop bound ensures */
        uint64_t rem = p - prod;
        if (rem % 7 != 0) continue;
        uint64_t x2 = rem / 7;
        if (is_square_uint64(x2)) return true;
    }
    return false;
}

/* Ensure prime_list can hold at least 'req' elements. */
static void
ensure_capacity(size_t req)
{
    if (prime_cap >= req) return;
    size_t new_cap = prime_cap ? prime_cap << 1 : 16;
    if (new_cap < req) new_cap = req;
    seq_t *tmp = (seq_t *)realloc(prime_list, new_cap * sizeof(seq_t));
    if (!tmp) abort();                 /* allocation failure */
    prime_list = tmp;
    prime_cap  = new_cap;
}

/* Append a new prime to the list. */
static void
add_prime(uint64_t p)
{
    ensure_capacity(prime_cnt + 1);
    prime_list[prime_cnt++] = (seq_t)p;
}

/* Generate at least 'need' terms. */
static void
generate_terms(size_t need)
{
    while (prime_cnt < need) {
        uint64_t cand = next_start;

        /* Skip even numbers greater than 2. */
        if (cand == 2) { /* 2 is not representable, just skip */
            cand = 3;
        } else if ((cand & 1) == 0) {
            cand++;
        }

        for (;;) {
            if (cand < 2) cand = 2;
            if ((cand & 1) == 0) { cand++; continue; }
            if (!is_prime_uint64(cand)) { cand++; continue; }
            if (representablePrime(cand)) {
                add_prime(cand);
                cand++;                       /* next search starts after this prime */
                break;
            }
            cand++;
        }
        next_start = cand;
    }
}

/* Public sequence function. */
seq_t
a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1 */
    generate_terms((size_t)n);
    return prime_list[n - 1];
}
