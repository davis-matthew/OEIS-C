/*
 * A073438  Remainder of division G[n]/Pi[n], where G[n] is the number of
 *          composites not exceeding n.
 * https://oeis.org/A073438
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Mod[n - Pi[n] - 1, Pi[n]] for n > 1
 * Range:     u8 n <= 377, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a simple incremental sieve to count primes,
   avoiding any external header dependencies. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define PRIME_CAP 200000      /* enough for n up to at least 10000 */

/* Static storage for prime numbers */
static idx_t  _primes[PRIME_CAP];
static idx_t  _prime_cnt   = 0;
static idx_t  _current_max = 1;

/* Ensure that all primes up to 'limit' are generated */
static void _ensure_primes_upto(idx_t limit)
{
    if (limit <= _current_max)
        return;

    for (idx_t cand = _current_max + 1; cand <= limit; ++cand) {
        bool is_prime = true;
        for (idx_t j = 0; j < _prime_cnt; ++j) {
            idx_t p = _primes[j];
            if (p * p > cand)
                break;
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (_prime_cnt < PRIME_CAP)
                _primes[_prime_cnt++] = cand;
            /* If PRIME_CAP is exceeded the sieve stops growing;
               for the intended use (n ≤ 10000) this never happens. */
        }
    }
    _current_max = limit;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                   /* offset starts at 2 */

    _ensure_primes_upto(n);
    seq_t pi = (seq_t)_prime_cnt;   /* Pi(n) */

    /* n, Pi(n) fit into seq_t for all n up to at least 10000 */
    seq_t composites = (seq_t)n - pi - 1;
    return composites % pi;
}
