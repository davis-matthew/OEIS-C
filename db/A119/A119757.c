/*
 * A119757  a(n) = n-th composite positive integer from among those composites
 *          which are coprime to the n-th composite.
 * https://oeis.org/A119757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method: sieve composites and then find the n‑th composite coprime to the n‑th composite. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"

struct {
    idx_t        max_needed;          /* largest n requested so far */
    idx_t        limit;               /* upper bound used for sieving */
    seq_t       *composites;          /* array of composites */
    size_t       composites_cnt;      /* number of composites stored */
} _a_state = {0, 0, NULL, 0};

static void _recompute_composites(idx_t need)
{
    /* --- choose an upper bound for the sieve --- */
    idx_t bound = need * 10 + 100;
    if (bound <= _a_state.limit) return;           /* already enough */

    _a_state.limit = bound;

    /* --- sieve to mark primes up to 'bound' --- */
    uint8_t *is_prime = (uint8_t *)calloc((size_t)(bound + 1), sizeof(uint8_t));
    if (!is_prime) exit(1);
    for (idx_t i = 2; i <= bound; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (idx_t p = 2; p * p <= bound; ++p) if (is_prime[p]) {
        for (idx_t k = p * p; k <= bound; k += p) is_prime[k] = 0;
    }

    /* --- collect composites into an array --- */
    free(_a_state.composites);
    seq_t *tmp = (seq_t *)malloc((size_t)(bound + 1) * sizeof(seq_t));
    if (!tmp) exit(1);
    size_t cnt = 0;
    for (idx_t i = 4; i <= bound; ++i)
        if (!is_prime[i]) tmp[cnt++] = (seq_t)i;

    _a_state.composites    = tmp;
    _a_state.composites_cnt = cnt;

    free(is_prime);
}

/* Ensure that the internal tables contain at least 'n' composites. */
static void _ensure_composites(idx_t n)
{
    if (_a_state.max_needed >= n) return;
    _a_state.max_needed = n;
    _recompute_composites(n);
    while (_a_state.composites_cnt < _a_state.max_needed) {
        idx_t new_n = _a_state.max_needed * 2;
        _recompute_composites(new_n);
    }
}

seq_t a(idx_t n)
{
    /* n is 1‑based */
    _ensure_composites(n);

    seq_t c = _a_state.composites[n-1];          /* n-th composite */
    idx_t target = n;                            /* we need the n‑th coprime */
    idx_t count  = 0;

    for (size_t i = 0; i < _a_state.composites_cnt; ++i) {
        if (gcd(_a_state.composites[i], c) == (seq_t)1) {
            ++count;
            if (count == target) return _a_state.composites[i];
        }
    }

    /* If the current limit did not contain enough coprime composites,
       extend the sieve and retry. */
    while (1) {
        idx_t new_limit = _a_state.limit * 2;
        _recompute_composites(new_limit);

        count = 0;
        for (size_t i = 0; i < _a_state.composites_cnt; ++i) {
            if (gcd(_a_state.composites[i], c) == (seq_t)1) {
                ++count;
                if (count == target) return _a_state.composites[i];
            }
        }
    }
}
