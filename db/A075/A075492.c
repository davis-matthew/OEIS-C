/*
 * A075492  Numbers k such that sum of digits of k < number of divisors of k.
 * https://oeis.org/A075492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the n-th integer k such that sum_of_digits(k) <
 *            divisor_count(k)
 * Range:     u8 n <= 52, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* <-- search algorithm: iterate increasing k and collect numbers satisfying the inequality */
/* The function a(idx_t n) returns a(n) with offset 1. */

#include <stdlib.h>
#include <stdint.h>

/* Compute the sum of decimal digits of a 64‑bit integer */
static uint32_t digit_sum_uint64(uint64_t x)
{
    uint32_t s = 0;
    while (x) {
        s += (uint32_t)(x % 10);
        x /= 10;
    }
    return s;
}

/* Compute the number of positive divisors of a 64‑bit integer */
static uint64_t divisor_count_uint64(uint64_t n)
{
    uint64_t cnt = 0;
    for (uint64_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            ++cnt;                 /* d is a divisor */
            if (d != n / d) ++cnt; /* n/d is a distinct divisor */
        }
    }
    return cnt;
}

/* Cache of computed sequence terms. */
static seq_t *terms = NULL;   /* array of terms, cast to seq_t format */
static size_t terms_len = 0;   /* number of terms stored */
static size_t terms_cap = 0;   /* capacity of the array */

/* Next integer to examine */
static idx_t next_candidate = 1;   /* start with 1; will be incremented before first use */

/* Public sequence function */
seq_t a(idx_t n)
{
    /* Return cached value if already known */
    if (terms_len >= n)
        return terms[n - 1];

    /* Generate terms until we have at least n of them */
    while (terms_len < n) {
        /* Move to the next candidate */
        uint64_t k = (uint64_t)next_candidate;
        ++next_candidate;

        /* Skip k < 1 because the sequence starts at 10 */
        if (k == 0)
            continue;

        /* Compute digit sum and divisor count */
        uint32_t ds = digit_sum_uint64(k);
        uint64_t tau = divisor_count_uint64(k);

        if (ds < tau) {
            /* Ensure enough capacity */
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* Allocation failed: abort */
                    exit(EXIT_FAILURE);
                }
                terms = tmp;
                terms_cap = new_cap;
            }
            /* Store the term (cast to seq_t; wraps naturally if seq_t is smaller) */
            terms[terms_len++] = (seq_t)k;
        }
    }

    /* Now the requested term is available */
    return terms[n - 1];
}
