/*
 * A129253  Number of numbers not greater than n having at least one divisor
 *          p^e with p<=e, p prime.
 * https://oeis.org/A129253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of integers m ≤ n that have a prime divisor p^e
 *            with exponent e of p satisfying e ≥ p
 * Range:     u8 n <= 919, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static idx_t  _max_n = 0;
static idx_t * _spf   = NULL;   /* smallest prime factor array */
static seq_t * _cum   = NULL;   /* cumulative counts of good numbers */

/* Return 1 if m has a divisor p^e with e ≥ p, else 0. */
static bool
good(idx_t m)
{
    if (m <= 1) return false;
    idx_t x = m;
    while (x > 1) {
        idx_t p = _spf[x];
        idx_t e = 0;
        while (x % p == 0) {
            x /= p;
            ++e;
        }
        if (e >= p) return true;
    }
    return false;
}

/* Compute a(n). */
seq_t
a(idx_t n)
{
    if (n == 0) return 0;

    /* Resize and recompute if necessary.  A full recomputation is
       inexpensive for the expected n (≈ 10000). */
    if (n > _max_n) {
        idx_t new_max = n;

        _spf = (idx_t *)realloc(_spf, (new_max + 1) * sizeof(idx_t));
        _cum = (seq_t *)realloc(_cum, (new_max + 1) * sizeof(seq_t));

        /* Initialize spf array. */
        for (idx_t i = 0; i <= new_max; ++i) {
            _spf[i] = i;
        }

        /* Sieve for smallest prime factors. */
        for (idx_t i = 2; i <= new_max / i; ++i) {
            if (_spf[i] == i) {
                for (idx_t j = i * i; j <= new_max; j += i) {
                    if (_spf[j] == j) {
                        _spf[j] = i;
                    }
                }
            }
        }

        /* Compute cumulative counts. */
        _cum[0] = 0;
        for (idx_t m = 1; m <= new_max; ++m) {
            _cum[m] = _cum[m - 1] + (good(m) ? 1 : 0);
        }

        _max_n = new_max;
    }

    return _cum[n];
}
