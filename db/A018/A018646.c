/*
 * A018646  Divisors of 786.
 * https://oeis.org/A018646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the nth divisor of 786 in ascending order
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define DIVCOUNT 8

/* List the prime factors of 786 and their exponents:
   786 = 2^1 * 3^1 * 131^1
*/
static const unsigned long long _primes[3] = {2ULL, 3ULL, 131ULL};
static const unsigned _exps[3]    = {1, 1, 1};

static seq_t _divisors[DIVCOUNT];
static int   _initialized = 0;

static void _init_divisors(void)
{
    size_t idx = 0;
    for (unsigned e0 = 0; e0 <= _exps[0]; ++e0) {
        for (unsigned e1 = 0; e1 <= _exps[1]; ++e1) {
            for (unsigned e2 = 0; e2 <= _exps[2]; ++e2) {
                unsigned long long value = 1ULL;
                for (unsigned i = 0; i < e0; ++i) value *= _primes[0];
                for (unsigned i = 0; i < e1; ++i) value *= _primes[1];
                for (unsigned i = 0; i < e2; ++i) value *= _primes[2];
                _divisors[idx++] = (seq_t)value;
            }
        }
    }
    /* Simple insertion sort (DIVCOUNT is 8, so O(1) time) */
    for (size_t i = 1; i < DIVCOUNT; ++i) {
        seq_t key = _divisors[i];
        size_t j = i;
        while (j > 0 && _divisors[j - 1] > key) {
            _divisors[j] = _divisors[j - 1];
            --j;
        }
        _divisors[j] = key;
    }
    _initialized = 1;
}

seq_t a(idx_t n)
{
    if (!_initialized) _init_divisors();
    if (n == 0) return (seq_t)0;
    if ((idx_t)DIVCOUNT >= n) return _divisors[n - 1];
    /* For indices beyond the number of divisors, return 0 */
    return (seq_t)0;
}
