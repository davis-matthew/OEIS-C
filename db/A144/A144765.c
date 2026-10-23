/*
 * A144765  Number of triprimes less than or equal to and not coprime to n.
 * https://oeis.org/A144765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of integers m ≤ n such that - m has exactly three
 *            prime factors (counted with multiplicity) - gcd(m, n) > 1 (OEIS
 *            A144765)
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd.h"

static idx_t _max_n = 0;          /* largest n processed so far                */
static idx_t _spf_limit = 0;      /* current size of the SPF array              */
static idx_t *_spf = NULL;        /* array holding smallest prime factor data   */
static seq_t *_trip = NULL;       /* list of triprimes (values ≤ _max_n)       */
static size_t _trip_size = 0;     /* number of stored triprimes                */
static size_t _trip_cap  = 0;     /* capacity of the trip array                */

/* Allocate or grow a dynamic array of seq_t elements */
static void _grow_trip(void) {
    if (_trip_cap == 0) {
        _trip_cap = 16;
        _trip = (seq_t *)malloc(_trip_cap * sizeof(seq_t));
    } else {
        _trip_cap <<= 1;
        _trip = (seq_t *)realloc(_trip, _trip_cap * sizeof(seq_t));
    }
}

/* Initialise or extend the SPF table up to `limit`.  The array is
   indexed from 0 to `limit`.  After the call _spf[i] equals the
   smallest prime factor of i for i ≥ 2. */
static void _init_spf(idx_t limit) {
    if (limit <= _spf_limit)
        return;

    /* Free old table (defensive) */
    free(_spf);
    _spf = (idx_t *)malloc((limit + 1) * sizeof(idx_t));
    if (!_spf)
        exit(EXIT_FAILURE);

    /* Initialise SPF values to the number itself. */
    _spf[0] = 0;
    _spf[1] = 1;
    for (idx_t i = 2; i <= limit; ++i)
        _spf[i] = i;

    /* Classic O(n log log n) sieve for SPF. */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (_spf[i] == i) {            /* i is prime */
            for (idx_t j = i * i; j <= limit; j += i) {
                if (_spf[j] == j)
                    _spf[j] = i;
            }
        }
    }
    _spf_limit = limit;
}

/* Return the number of prime factors (with multiplicity) of x,
   but stop counting once we exceed 3. */
static int _pf_count(idx_t x) {
    int cnt = 0;
    while (x > 1 && cnt <= 3) {
        x /= _spf[x];
        ++cnt;
    }
    return cnt;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0)         /* safety, the sequence is defined for n ≥ 1 */
        return (seq_t)0;

    /* Ensure SPF table and trip list are up to date. */
    if (n > _max_n) {
        _init_spf(n);
        for (idx_t i = _max_n + 1; i <= n; ++i) {
            if (_pf_count(i) == 3) {          /* triprime found */
                _grow_trip();
                _trip[_trip_size++] = (seq_t)i;
            }
        }
        _max_n = n;
    }

    seq_t result = 0;
    seq_t n_seq = (seq_t)n;

    /* Count triprimes ≤ n that are not coprime to n. */
    for (size_t i = 0; i < _trip_size; ++i) {
        seq_t t = _trip[i];
        if (t > n_seq)
            break;
        if (gcd(t, n_seq) > (seq_t)1)
            ++result;
    }
    return result;
}
