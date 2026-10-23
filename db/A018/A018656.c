/*
 * A018656  Divisors of 805.
 * https://oeis.org/A018656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Divisors of 805 in ascending order (a(1)=1, a(8)=805).  */

#include <stddef.h>
#include <stdbool.h>

static bool _A018656_initialized = false;
static size_t _A018656_div_count = 0;
static seq_t _A018656_divs[8];   /* 8 divisors of 805 */

/* Fill the array with all divisors of 805 and sort them. */
static void _A018656_compute_divisors(void)
{
    const uint64_t N = 805ULL;
    uint64_t primes[3];
    size_t pc = 0;
    uint64_t tmp = N;

    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            primes[pc++] = p;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) {
        primes[pc++] = tmp;
    }

    /* Generate divisors via subset enumeration. */
    size_t total = 1ULL << pc;
    for (size_t mask = 0; mask < total; ++mask) {
        seq_t prod = 1;
        for (size_t i = 0; i < pc; ++i) {
            if (mask & (1ULL << i)) {
                prod *= (seq_t)primes[i];
            }
        }
        _A018656_divs[mask] = prod;
    }

    /* Simple bubble sort to order the divisors ascending. */
    for (size_t i = 0; i < total; ++i) {
        for (size_t j = i + 1; j < total; ++j) {
            if (_A018656_divs[j] < _A018656_divs[i]) {
                seq_t tmp = _A018656_divs[i];
                _A018656_divs[i] = _A018656_divs[j];
                _A018656_divs[j] = tmp;
            }
        }
    }

    _A018656_div_count = total;
    _A018656_initialized = true;
}

seq_t a(idx_t n)
{
    if (!_A018656_initialized) {
        _A018656_compute_divisors();
    }
    if (n >= (idx_t)1 && ((size_t)(n - 1)) < _A018656_div_count) {
        return _A018656_divs[(size_t)(n - 1)];
    }
    return (seq_t)0;  /* undefined for n > 8 */
}
