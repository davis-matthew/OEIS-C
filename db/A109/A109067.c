/*
 * A109067  3-almost primes of the form semiprime + 1.
 * https://oeis.org/A109067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   omega_uint64, push_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A109067 – 3‑almost primes of the form semiprime + 1
   A(n) is the n‑th positive integer x such that
   - ω(x) = 3 (exactly three prime factors, counted with multiplicity)
   - x – 1 is a semiprime (product of two primes, not necessarily distinct)   */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/push_seq.h"
#include "../utils/omega_uint64.h"

static seq_t *terms   = NULL;   /* array of computed terms      */
static idx_t  terms_len = 0;    /* number of terms stored       */
static idx_t  terms_cap = 0;    /* current capacity of array    */
static uint64_t candidate = 1;  /* next integer to test         */
static bool initialized = false;

/* Test whether x is a semiprime (product of two primes) */
static bool is_semiprime_uint64(uint64_t x)
{
    if (x < 4) return false;      /* 2·2 is the smallest semiprime */
    int cnt = 0;
    while ((x & 1ULL) == 0ULL) { /* factor 2 */
        ++cnt;
        x >>= 1;
        if (cnt > 2) return false;
    }
    for (uint64_t p = 3; p <= x / p; p += 2) {
        while (x % p == 0) {
            ++cnt;
            x /= p;
            if (cnt > 2) return false;
        }
    }
    if (x > 1) ++cnt;
    return cnt == 2;
}

/* Main function required by the build. */
seq_t a(idx_t n)
{
    if (!initialized) {
        initialized = true;
        terms_cap = 64;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        terms_len = 0;
    }

    while (terms_len < n) {
        ++candidate;
        if (omega_uint64(candidate) == 3 && is_semiprime_uint64(candidate - 1)) {
            push_seq(&terms, &terms_cap, &terms_len, (seq_t)candidate);
        }
    }
    return terms[n - 1];
}
