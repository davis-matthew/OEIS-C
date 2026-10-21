/*
 * A056675  Number of non-unitary but squarefree divisors of n!. Also number
 *          of unitary but not-squarefree divisors of n!.
 * https://oeis.org/A056675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^π(n) - 2^(π(n)-π(floor(n/2)))
 * Range:     u8 n <= 22, u16 n <= 58, u32 n <= 136, u64 n <= 312,
 *            u128 n <= 726, big any n
 * Helpers:   _pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* π(k) is the number of primes ≤ k.  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/_pow2.h"

/* Static sieve data */
static size_t _max_sieve = 0;          /* largest n that has been sieved */
static uint8_t *_sieve      = NULL;    /* 1 = prime, 0 = composite */
static uint32_t *_pi        = NULL;    /* prefix counts of primes */

/* Ensure that primes up to 'limit' are sieved and _pi[limit] is valid. */
static void _ensure_sieve(idx_t limit)
{
    if ((size_t)limit <= _max_sieve)
        return;                       /* already done */

    size_t new_sz = (size_t)limit + 1;
    uint8_t *new_sieve = (uint8_t *)realloc(_sieve, new_sz);
    uint32_t *new_pi    = (uint32_t *)realloc(_pi, new_sz * sizeof(uint32_t));
    if (!new_sieve || !new_pi) {
        abort();                       /* out of memory */
    }
    _sieve = new_sieve;
    _pi    = new_pi;

    /* initialise as prime except 0 and 1 */
    for (size_t i = 0; i < new_sz; ++i) {
        _sieve[i] = (i >= 2) ? 1 : 0;
    }

    /* simple sieve of Eratosthenes */
    size_t limit_sqrt = (size_t)floor(sqrt((double)new_sz)) + 1;
    for (size_t p = 2; p < limit_sqrt; ++p) {
        if (_sieve[p]) {
            for (size_t m = p * p; m < new_sz; m += p)
                _sieve[m] = 0;
        }
    }

    /* prefix prime counts */
    uint32_t cnt = 0;
    for (size_t i = 0; i < new_sz; ++i) {
        if (_sieve[i]) ++cnt;
        _pi[i] = cnt;
    }

    _max_sieve = limit;
}

/* Sequence function defined by the OEIS record. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined, but safe */

    /* Build sieve up to n. */
    _ensure_sieve(n);

    /* π(n) and π(floor(n/2)) */
    uint32_t pi_n      = _pi[n];
    uint32_t pi_half   = _pi[n / 2];

    /* 2^π(n)  and  2^(π(n)-π(n/2)) */
    seq_t pow_pi_n           = _pow2(pi_n);
    seq_t pow_pi_diff        = _pow2((idx_t)(pi_n - pi_half));

    return pow_pi_n - pow_pi_diff;
}
