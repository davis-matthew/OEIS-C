/*
 * A018780  Divisors of 1022.
 * https://oeis.org/A018780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: divisors of 1022 in ascending order (OEIS A018780) */
#include <stdbool.h>
#include <stddef.h>

static const unsigned long long _primes[3] = {2ULL, 7ULL, 73ULL};

static seq_t _divs[8];
static bool _divs_ready = false;

seq_t a(idx_t n)
{
    if (! _divs_ready) {
        for (idx_t mask = 0; mask < 8; ++mask) {
            seq_t prod = 1;
            for (idx_t i = 0; i < 3; ++i)
                if (mask & (1ULL << i))
                    prod *= (seq_t)_primes[i];
            _divs[mask] = prod;
        }
        _divs_ready = true;
    }

    if (n == 0 || n > 8)
        return (seq_t)0;   /* undefined for n <= 0 or n > 8 */

    return _divs[n-1];
}
