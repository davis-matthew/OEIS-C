/*
 * A107000  Primes of the form 2x^2-xy+12y^2, with x and y nonnegative.
 * https://oeis.org/A107000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes of the form 2*x^2 - x*y + 12*y^2 (x, y ≥ 0)
 * Range:     u8 none, u16 none, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* Helper: compare two seq_t values for qsort */
static int cmp_seq(const void *a, const void *b)
{
    seq_t v1 = *(const seq_t *)a;
    seq_t v2 = *(const seq_t *)b;
    return (v1 > v2) - (v1 < v2);
}

/* Generate all primes ≤ B that are represented by the quadratic form
   2*x^2 - x*y + 12*y^2, and store them sorted ascending in *out */
static void generate_primes(uint64_t B, seq_t **out, size_t *len)
{
    /* Sieve of Eratosthenes up to B (byte per candidate) */
    size_t sz = (size_t)B + 1;
    uint8_t *is_prime = (uint8_t *)malloc(sz);
    if (!is_prime) abort();
    for (size_t i = 2; i < sz; ++i) is_prime[i] = 1;
    for (size_t p = 2; p * p <= B; ++p) {
        if (is_prime[p]) {
            for (size_t m = p * p; m <= B; m += p) is_prime[m] = 0;
        }
    }

    /* Estimate upper bound on number of such primes – rough */
    size_t cap = (size_t)(B / 100) + 1;          /* very generous */
    seq_t *buf = alloc_seq_array(cap);
    if (!buf) abort();
    size_t cnt = 0;

    /* Enumerate all representations up to B */
    for (uint64_t y = 0; 12 * y * y <= B; ++y) {
        for (uint64_t x = 0;; ++x) {
            uint64_t val = 2 * x * x - x * y + 12 * y * y;
            if (val > B) break;
            if (val >= 2 && is_prime[(size_t)val]) {
                if (cnt == cap) {
                    /* Grow buffer if needed */
                    cap += cap / 2 + 1;
                    seq_t *tmp = alloc_seq_array(cap);
                    if (!tmp) abort();
                    for (size_t i = 0; i < cnt; ++i) tmp[i] = buf[i];
                    free_seq_array(buf);
                    buf = tmp;
                }
                buf[cnt++] = (seq_t)val;
            }
        }
    }

    free(is_prime);

    /* Sort the primes */
    qsort(buf, cnt, sizeof(seq_t), cmp_seq);

    *out = buf;
    *len = cnt;
}

/* The public sequence function */
seq_t a(idx_t n)
{
    /* static state */
    static seq_t *values = NULL;
    static size_t values_len = 0;

    /* Build the list once if not yet built */
    if (!values) {
        /* Choose a bound that covers at least 10 000 terms comfortably.
           50 million is more than enough for the tested range. */
        const uint64_t B = 50000000ULL;
        seq_t *tmp;
        size_t len;
        generate_primes(B, &tmp, &len);
        values      = tmp;
        values_len  = len;
    }

    if (n >= 1 && (idx_t)(n - 1) < values_len)
        return values[(size_t)(n - 1)];

    /* For safety – return 0 if n is out of the precomputed range.
       The build is expected to cover all requested indices. */
    return (seq_t)0;
}
