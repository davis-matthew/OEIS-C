/*
 * A021216  Decimal expansion of 1/212.
 * https://oeis.org/A021216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/212) mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

static bool _a021216_generated = false;
static seq_t _a021216_digits[212];
static size_t _a021216_nb_digits;
static size_t _a021216_prefix;
static size_t _a021216_period;

static void _a021216_generate(void) {
    const uint64_t D = 212;
    int first[212];
    for (int i = 0; i < 212; ++i) first[i] = -1;

    uint64_t remainder = 1 % D;
    size_t i = 0;
    while (1) {
        int pos = (int)remainder;
        if (first[pos] != -1) {                 /* cycle detected */
            _a021216_prefix = (size_t)first[pos];
            _a021216_period  = i - _a021216_prefix;
            _a021216_nb_digits = i;
            break;
        }
        first[pos] = (int)i;
        uint64_t next = remainder * 10ull;
        seq_t digit = (seq_t)(next / D);
        _a021216_digits[i] = digit;
        remainder = next % D;
        ++i;
    }
    _a021216_generated = true;
}

seq_t a(idx_t n) {
    if (!_a021216_generated)
        _a021216_generate();

    if (n < (idx_t)_a021216_nb_digits)
        return _a021216_digits[(size_t)n];

    idx_t pref   = (idx_t)_a021216_prefix;
    idx_t period = (idx_t)_a021216_period;
    idx_t idx    = pref + (n - pref) % period;
    return _a021216_digits[(size_t)idx];
}
