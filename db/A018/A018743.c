/*
 * A018743  Divisors of 957.
 * https://oeis.org/A018743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = sorted divisors of 957 (with offset 1)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

static seq_t _divisors[8];
static bool _init_done = false;

static void _init_divisors(void)
{
    if (_init_done)
        return;

    const seq_t primes[3] = { (seq_t)3, (seq_t)11, (seq_t)29 };
    size_t pos = 0;

    for (int i = 0; i <= 1; ++i)
        for (int j = 0; j <= 1; ++j)
            for (int k = 0; k <= 1; ++k)
            {
                seq_t val = 1;
                if (i) val *= primes[0];
                if (j) val *= primes[1];
                if (k) val *= primes[2];
                _divisors[pos++] = val;
            }

    /* simple insertion sort (there are only 8 elements) */
    for (size_t a = 1; a < 8; ++a)
        for (size_t b = 0; b < a; ++b)
            if (_divisors[a] < _divisors[b])
            {
                seq_t tmp = _divisors[a];
                _divisors[a] = _divisors[b];
                _divisors[b] = tmp;
            }

    _init_done = true;
}

seq_t a(idx_t n)
{
    if (n < 1 || n > 8)
        return (seq_t)0; /* undefined for out‑of‑range n */

    _init_divisors();
    return _divisors[n - 1];
}
