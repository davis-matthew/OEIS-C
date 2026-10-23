/*
 * A115058  Primes p such that 3*p+2 is not prime.
 * https://oeis.org/A115058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the primes p such that 3*p+2 is composite
   (OEIS A115058) */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"          /* bool is_prime(idx_t x); */
#include "../utils/build_primes_upto.h" /* void build_primes_upto(idx_t limit,
                                         uint64_t **primes_out, size_t *cnt_out); */

/* Ensure that the output array has at least `new_cap` slots.  */
static void _ensure_output_capacity(size_t new_cap, seq_t **output, size_t *cap)
{
    if (new_cap > *cap)
    {
        *cap = new_cap * 2;
        seq_t *tmp = (seq_t *)realloc(*output, *cap * sizeof(seq_t));
        *output = tmp;
    }
}

/* Build all primes up to `limit` and store those p with 3*p+2 composite
   into `output`.  Resulting `out_cnt` is the number of terms produced.  */
static void _build_output(idx_t limit,
                          seq_t **output,
                          size_t *out_cnt,
                          size_t *out_cap)
{
    /* Generate all primes up to limit. */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(limit, &primes, &prime_cnt);

    /* Reset output buffer. */
    free(*output);
    *output = NULL;
    *out_cnt = 0;
    *out_cap = 0;

    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p = primes[i];
        if (!is_prime(3 * p + 2))              /* 3*p+2 composite */
        {
            _ensure_output_capacity(*out_cnt + 1, output, out_cap);
            (*output)[(*out_cnt)++] = (seq_t)p;
        }
    }
    free(primes);
}

seq_t a(idx_t n)
{
    static bool initialized = false;
    static idx_t current_limit = 250000;   /* initial sieve limit */
    static seq_t *output = NULL;
    static size_t out_cnt = 0, out_cap = 0;

    if (!initialized)
    {
        _build_output(current_limit, &output, &out_cnt, &out_cap);
        initialized = true;
    }

    /* If the requested index exceeds available terms, enlarge the sieve. */
    while (out_cnt < n)
    {
        current_limit = current_limit * 2 + 1000;   /* grow limit */
        _build_output(current_limit, &output, &out_cnt, &out_cap);
    }

    return output[n - 1];   /* OEIS offset is 1 */
}
