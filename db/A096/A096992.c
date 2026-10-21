/*
 * A096992  Initial values for f(x)=sigma(phi[x])=A062402[x] such that
 *          iteration of f ends in cycle of length=6.
 * https://oeis.org/A096992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   calc_phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A096992
   a(n) is the nth smallest integer x such that the iteration of  
   f(x) = σ(φ(x)) eventually enters a cycle of length 6. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "../utils/calc_phi.h"
#include "../utils/sigma_of.h"

/* Transform x → σ(φ(x)) */
static inline seq_t f_value(seq_t x)
{
    seq_t phi = calc_phi(x);
    return sigma_of((idx_t)phi);
}

/* Returns true iff the orbit of start under f_value has cycle length 6 */
static bool is_cycle_len6(seq_t start)
{
    /* Floyd's cycle–finding algorithm */
    seq_t tortoise = f_value(start);
    seq_t hare     = f_value(f_value(start));

    while (tortoise != hare) {
        tortoise = f_value(tortoise);
        hare     = f_value(f_value(hare));
    }

    /* Find the start of the cycle (mu) */
    seq_t mu = start;
    while (mu != tortoise) {
        mu       = f_value(mu);
        tortoise = f_value(tortoise);
    }

    /* Determine the cycle length (lambda) */
    seq_t cur = f_value(mu);
    size_t lambda = 1;
    while (cur != mu) {
        cur = f_value(cur);
        ++lambda;
        if (lambda > 1000)    /* safety guard */
            return false;
    }

    return lambda == 6;
}

/* Cache of discovered terms */
static seq_t *  _cache          = NULL;
static size_t   _cap            = 0;
static size_t   _found          = 0;
static seq_t   _search_start    = (seq_t)1;

/* Grow the cache if needed and find terms up to at least n */
static void _ensure_cache(idx_t n)
{
    while (_found < (size_t)n) {
        seq_t candidate = _search_start++;

        if (is_cycle_len6(candidate)) {
            /* Grow cache if full */
            if (_found == _cap) {
                size_t new_cap = (_cap == 0) ? 16 : _cap * 2;
                seq_t *tmp = (seq_t *)realloc(_cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                _cache = tmp;
                _cap   = new_cap;
            }
            _cache[_found++] = candidate;
        }
    }
}

/* External function required by the build */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */
    _ensure_cache(n);
    return _cache[(size_t)n - 1];
}
