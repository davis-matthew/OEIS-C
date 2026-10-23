/*
 * A035303  Divisors of 3600.
 * https://oeis.org/A035303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: divisors of 3600 in increasing order */
/* a(1)=1, a(2)=2, a(3)=3, a(4)=4, ... , a(45)=3600 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define DIVISOR_COUNT 45

static seq_t _divisors[DIVISOR_COUNT];
static bool _divisors_initialized = false;

static void _sort_divisors(void)
{
    /* simple insertion sort – works for any seq_t type */
    for (size_t i = 1; i < DIVISOR_COUNT; ++i) {
        for (size_t j = i; j > 0 && _divisors[j] < _divisors[j-1]; --j) {
            seq_t tmp = _divisors[j];
            _divisors[j] = _divisors[j-1];
            _divisors[j-1] = tmp;
        }
    }
}

static void _init_divisors(void)
{
    if (_divisors_initialized) return;

    size_t pos = 0;
    for (int a = 0; a <= 4; ++a) {          /* exponent of 2 */
        uint64_t val2 = 1;
        for (int i = 0; i < a; ++i) val2 *= 2;

        for (int b = 0; b <= 2; ++b) {      /* exponent of 3 */
            uint64_t val3 = val2;
            for (int j = 0; j < b; ++j) val3 *= 3;

            for (int c = 0; c <= 2; ++c) {  /* exponent of 5 */
                uint64_t v = val3;
                for (int k = 0; k < c; ++k) v *= 5;
                _divisors[pos++] = (seq_t) v;
            }
        }
    }

    _sort_divisors();
    _divisors_initialized = true;
}

seq_t a(idx_t n)
{
    _init_divisors();

    if (n >= 1 && n <= DIVISOR_COUNT) {
        return _divisors[n - 1];
    }
    /* the sequence is finite – out of range values are undefined */
    return (seq_t)0;
}
