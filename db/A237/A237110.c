/*
 * A237110  Maximum number of distinct prime factors of pairs of coprime g and
 *          h (g < h) adding to n.
 * https://oeis.org/A237110
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max{ ω(g)+ω(h) | g+h=n, 1≤g<h, gcd(g,h)=1 } where ω(m) is
 *            the number of distinct prime factors of m. (OEIS A237110)
 * Range:     u8 n <= 1549, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/build_primes_upto.h"

/* global cache: biggest n processed so far */
static idx_t _max_n = 0;
/* array of ω values (index 0..n) */
static seq_t *_omega = NULL;
/* array of memoised a(n) values; ~(seq_t)0 means "unknown" */
static seq_t *_cache = NULL;

/* initialise or grow the tables so that indices 0..n are available */
static void _init_up_to(idx_t n)
{
    if (n <= _max_n)
        return;                     /* already covered */

    /* free any previous tables */
    free(_omega);
    free(_cache);

    /* allocate omega array, zero initialised */
    _omega = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (_omega == NULL)
        return;                     /* allocation failure */

    /* obtain all primes up to n */
    uint64_t *primes = NULL;
    size_t primes_cnt = 0;
    build_primes_upto(n, &primes, &primes_cnt);

    /* sieve: count distinct prime factors */
    for (size_t pi = 0; pi < primes_cnt; ++pi) {
        uint64_t p = primes[pi];
        for (idx_t m = p; m <= n; m += p)
            _omega[m] = _omega[m] + (seq_t)1;
    }
    free(primes);                    /* clean up prime list */

    /* allocate cache array, fill with the sentinel ~(seq_t)0 */
    _cache = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (_cache != NULL) {
        seq_t sentinel = ~(seq_t)0;
        for (idx_t i = 0; i <= n; ++i)
            _cache[i] = sentinel;
    }

    _max_n = n;
}

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;              /* below offset, value unused */

    _init_up_to(n);
    if (!_omega || !_cache)
        return (seq_t)0;              /* failed initialisation */

    /* a sentinel value indicating "not yet computed" */
    seq_t sentinel = ~(seq_t)0;
    if (_cache[n] != sentinel)
        return _cache[n];

    seq_t best = (seq_t)0;
    for (idx_t g = 1; g * 2 < n; ++g) {      /* ensures g < h automatically */
        idx_t h = n - g;
        if (gcd((seq_t)g, (seq_t)h) == (seq_t)1) {
            seq_t val = _omega[g] + _omega[h];
            if (val > best)
                best = val;
        }
    }

    _cache[n] = best;                     /* memoise */
    return best;
}
