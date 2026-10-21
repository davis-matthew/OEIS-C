/*
 * A090883  Write n as Product_{i=1..k} prime(i)^e_i, where prime(i) is the
 *          i-th prime number and e_i is a nonnegative integer. a(n) =
 *          Sum_{i=1..k} e_i*n^(i-1).
 * https://oeis.org/A090883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ e_i * n^(i-1) where n = Π p_i^e_i (p_i is the i-th
 *            prime)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   _pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/_pow_seq.h"

/* ---------- prime table --------------------------------------------------- */
static idx_t     _last_prime_limit = 0; /* largest n for which primes are generated  */
static size_t    _prime_cnt        = 0; /* number of primes stored                */
static uint64_t *_prime_tbl        = NULL; /* array of prime values                 */

/* Generate all primes up to 'limit' (inclusive) and store them in _prime_tbl.
   If already generated up to a larger limit, nothing is done. */
static void _ensure_primes_upto(idx_t limit)
{
    if (limit <= _last_prime_limit) return;

    /* Simple sieve of Eratosthenes up to limit */
    size_t sz = (size_t)limit + 1;
    bool *is_comp = (bool *)calloc(sz, sizeof(bool));
    if (!is_comp) exit(1);

    /* allocate initial space for primes, grow as needed */
    size_t prime_cap = limit / 10 + 10;            /* rough estimate            */
    _prime_tbl = (uint64_t *)realloc(_prime_tbl,
                                     prime_cap * sizeof(uint64_t));
    if (!_prime_tbl) { free(is_comp); exit(1); }

    size_t cnt = 0;
    for (idx_t p = 2; p <= limit; ++p) {
        if (!is_comp[p]) {
            if (cnt >= prime_cap) {               /* grow array if full     */
                prime_cap *= 2;
                _prime_tbl = (uint64_t *)realloc(_prime_tbl,
                                                prime_cap * sizeof(uint64_t));
                if (!_prime_tbl) { free(is_comp); exit(1); }
            }
            _prime_tbl[cnt++] = (uint64_t)p;

            if ((uint64_t)p * p <= limit) {        /* mark composites       */
                for (idx_t q = p * p; q <= limit; q += p)
                    is_comp[q] = true;
            }
        }
    }
    _prime_cnt          = cnt;
    _last_prime_limit   = limit;
    free(is_comp);
}

/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for 0, but safe fallback */

    _ensure_primes_upto(n);      /* primes up to n are needed for factorization */

    seq_t   total = 0;
    uint64_t m    = (uint64_t)n;
    uint64_t tmp  = m;

    /* trial division using the prime table */
    for (size_t i = 0; i < _prime_cnt && (uint64_t)_prime_tbl[i] * 
           (uint64_t)_prime_tbl[i] <= tmp; ++i) {

        uint64_t p = _prime_tbl[i];
        if (tmp % p == 0) {
            uint64_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            seq_t term = (seq_t)exp * _pow_seq((seq_t)m, (idx_t)i);
            total += term;
        }
    }

    /* any remaining factor is a prime > sqrt(n) */
    if (tmp > 1) {
        /* binary search for its index in _prime_tbl */
        size_t lo = 0, hi = _prime_cnt, idx = _prime_cnt;
        while (lo < hi) {
            size_t mid = (lo + hi) / 2;
            if (_prime_tbl[mid] == tmp) { idx = mid; break; }
            if (_prime_tbl[mid] <  tmp) lo = mid + 1;
            else                        hi = mid;
        }
        /* idx is zero‑based index of the prime */
        seq_t term = (seq_t)1 * _pow_seq((seq_t)m, (idx_t)idx);
        total += term;
    }

    return total;
}
