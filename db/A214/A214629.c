/*
 * A214629  Primes p such that the sum of the digits plus the product of the
 *          digits is a prime.
 * https://oeis.org/A214629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 1803, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, digit_product, digit_sum, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the nth prime such that digit_sum(p)+digit_product(p) is prime.  */
#include <stdint.h>
#include <stdlib.h>
#ifndef __cplusplus
#include <stdbool.h>
#endif

#include "../utils/digit_sum.h"
#include "../utils/digit_product.h"
#include "../utils/is_prime_small.h"
#include "../utils/append_candidate.h"

static bool _seq_a_init_done = false;
/* Adjust this limit if higher indices are needed. */
static const size_t _SEQ_A_MAX_LIMIT = 5000000;

static bool *_SEQ_A_PRIME_MARK = NULL;
static uint32_t *_SEQ_A_PRIME_LIST = NULL;
static size_t _SEQ_A_PRIME_COUNT = 0;
static size_t _SEQ_A_PRIME_IDX = 0;

static seq_t *_SEQ_A_VALUES = NULL;
static size_t _SEQ_A_VALUES_LEN = 0;
static size_t _SEQ_A_VALUES_CAP = 0;

/* Build a list of all primes up to _SEQ_A_MAX_LIMIT. */
static void _seq_a_precompute(void)
{
    if (_seq_a_init_done)
        return;

    const size_t limit = _SEQ_A_MAX_LIMIT;
    _SEQ_A_PRIME_MARK = (bool *)malloc((limit + 1) * sizeof(bool));
    for (size_t i = 0; i <= limit; ++i)
        _SEQ_A_PRIME_MARK[i] = true;
    _SEQ_A_PRIME_MARK[0] = _SEQ_A_PRIME_MARK[1] = false;

    for (size_t i = 2; i * i <= limit; ++i) {
        if (_SEQ_A_PRIME_MARK[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                _SEQ_A_PRIME_MARK[j] = false;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (_SEQ_A_PRIME_MARK[i])
            ++cnt;
    _SEQ_A_PRIME_COUNT = cnt;

    _SEQ_A_PRIME_LIST = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (_SEQ_A_PRIME_MARK[i])
            _SEQ_A_PRIME_LIST[idx++] = (uint32_t)i;

    free(_SEQ_A_PRIME_MARK);
    _SEQ_A_PRIME_MARK = NULL;

    _seq_a_init_done = true;
}

/* Return the nth term of the sequence (1‑based).  */
seq_t a(idx_t n)
{
    _seq_a_precompute();
    if (n == 0)
        return 0; /* undefined for offset 1 */

    /* If we already have the result cached, return it. */
    if (_SEQ_A_VALUES_LEN >= (size_t)n)
        return _SEQ_A_VALUES[(size_t)n - 1];

    /* Search for additional terms until we have at least n. */
    while (_SEQ_A_VALUES_LEN < (size_t)n) {
        if (_SEQ_A_PRIME_IDX >= _SEQ_A_PRIME_COUNT)
            return 0; /* insufficient primes pre‑computed */

        uint64_t p = (uint64_t)_SEQ_A_PRIME_LIST[_SEQ_A_PRIME_IDX++];
        seq_t sum = digit_sum((seq_t)p);
        seq_t prod = digit_product((seq_t)p);
        seq_t sp = sum + prod;

        if (is_prime_small(sp))
            /* Store the prime p itself, not the sum+product. */
            append_candidate(&_SEQ_A_VALUES, &_SEQ_A_VALUES_LEN,
                              &_SEQ_A_VALUES_CAP, (seq_t)p);
    }

    return _SEQ_A_VALUES[(size_t)n - 1];
}
