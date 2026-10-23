/*
 * A018727  Divisors of 930.
 * https://oeis.org/A018727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 930: a(n) is the n‑th smallest positive divisor of 930
   (OEIS A018727).  The primes of 930 are 2, 3, 5, 31, so there are
   2^4 = 16 divisors. */

#include <stdbool.h>
#include <stddef.h>

/* Prime factors of 930 */
static const seq_t _primes[4] = { 2, 3, 5, 31 };

/* Number of divisors of 930 (2^4) */
#define _DIV_CNT 16

/* Cache of the sorted divisors.  Must be filled once. */
static seq_t _divisors[_DIV_CNT];
static bool _divisors_initialized = false;

/* Helper: generate all divisors and sort them ascending. */
static void _fill_divisors(void)
{
    if (_divisors_initialized)
        return;

    /* Enumerate all subsets of the prime set. */
    for (size_t mask = 0; mask < (size_t)_DIV_CNT; ++mask) {
        seq_t prod = 1;
        for (size_t i = 0; i < 4; ++i)
            if (mask & (1U << i))
                prod *= _primes[i];
        _divisors[mask] = prod;
    }

    /* Simple insertion sort – works for such a small array. */
    for (size_t i = 1; i < (size_t)_DIV_CNT; ++i) {
        seq_t val = _divisors[i];
        size_t j = i;
        while (j > 0 && _divisors[j - 1] > val) {
            _divisors[j] = _divisors[j - 1];
            --j;
        }
        _divisors[j] = val;
    }

    _divisors_initialized = true;
}

/* Public entry point required by the test harness. */
seq_t a(idx_t n)
{
    /* Return 0 for indices outside the defined range. */
    if (n < 1 || n > (idx_t)_DIV_CNT)
        return 0;

    _fill_divisors();
    /* Convert idx_t to size_t safely for array indexing. */
    return _divisors[(size_t)(n - 1)];
}
