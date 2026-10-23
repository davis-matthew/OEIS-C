/*
 * A137670  Prime numbers p such that p-b < p-a < p < p+a < p+b are prime for
 *          some a and b.
 * https://oeis.org/A137670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime p such that there exist two distinct *
 *            positive integers a<b, both divisible by 6, with all of * p‑b ,
 *            p‑a , p , p + a , p + b prime
 * Range:     u8 n <= 46, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime.h"

/* static storage for the pre‑computed sequence */
static bool            _initialized = false;
static seq_t *         _qualifiers  = NULL;   /* array of qualifying primes */
static size_t          _qual_cap    = 0;       /* current capacity          */
static size_t          _qual_len    = 0;       /* number of elements stored */

/* helper: ensure the qualifier array has the needed capacity */
static void _ensure_qual_capacity(size_t needed)
{
    if (needed <= _qual_cap)
        return;
    size_t new_cap = (_qual_cap == 0) ? 16 : _qual_cap << 1;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(_qualifiers, new_cap * sizeof(seq_t));
    if (!tmp && new_cap) {
        /* out of memory – abort in a portable way */
        abort();
    }
    _qualifiers = tmp;
    _qual_cap = new_cap;
}

/* build the sequence once; expands the sieve limit until ~10 000 terms
 * are collected (the sequence contains almost all primes). */
static void _prepare(void)
{
    if (_initialized)
        return;

    /* start with a sieve large enough for our needs; 500 000
     * primes are more than enough to get >10 000 qualified primes. */
    idx_t limit = 500000ULL;

    while (true) {
        uint64_t *primes = NULL;
        size_t    prime_cnt = 0;

        build_primes_upto(limit, &primes, &prime_cnt);
        if (!primes || prime_cnt == 0) {
            free(primes);
            abort();
        }

        _ensure_qual_capacity(prime_cnt);
        _qual_len = 0;

        for (size_t i = 0; i < prime_cnt; ++i) {
            seq_t p = (seq_t)primes[i];
            int   matches = 0;

            for (size_t j = 0; j < i && matches < 2; ++j) {
                seq_t q = (seq_t)primes[j];
                seq_t d = p - q;            /* distance from p to the lower prime */

                /* we only accept distances that are multiples of 6 */
                if ((d % 6u) != 0)
                    continue;

                /* check the corresponding upper prime */
                if (is_prime((idx_t)(p + d)))
                    ++matches;
            }

            if (matches >= 2) {
                _qualifiers[_qual_len++] = p;
            }
        }

        free(primes);

        /* we always have more than 10 000 qualifying primes once the
         * sieve limit exceeds a few hundred thousand */
        if (_qual_len >= 10000)
            break;
        /* not enough – double the sieve limit and retry */
        limit <<= 1;
    }

    _initialized = true;
}

/* external linkage function required by the harness */
seq_t a(idx_t n)
{
    _prepare();            /* guarantee that the sequence is ready */
    return _qualifiers[n - 1];
}
