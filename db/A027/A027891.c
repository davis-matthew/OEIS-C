/*
 * A027891  Divisors of 9999999.
 * https://oeis.org/A027891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth divisor of 9999999 (3²·239·4649),
   sorted in increasing order.  The divisors are generated once
   and stored in a static array.  a(1) is 1, a(12) is 9999999. */

#include <stdbool.h>
#include <stddef.h>

static seq_t _divisors[12];
static bool _divisors_ready = false;

static void _init_divisors(void)
{
    size_t pos = 0;
    /* 3² · 239 · 4649 = 9999999 */
    for (int e3 = 0; e3 <= 2; ++e3) {
        seq_t v3 = 1;
        for (int i = 0; i < e3; ++i)
            v3 = v3 * (seq_t)3;

        for (int e239 = 0; e239 <= 1; ++e239) {
            seq_t v239 = v3;
            if (e239)
                v239 = v239 * (seq_t)239;

            for (int e4649 = 0; e4649 <= 1; ++e4649) {
                seq_t val = v239;
                if (e4649)
                    val = val * (seq_t)4649;

                _divisors[pos++] = val;
            }
        }
    }

    /* Simple insertion sort – only 12 values */
    for (size_t i = 1; i < 12; ++i) {
        seq_t key = _divisors[i];
        size_t j = i;
        while (j > 0 && _divisors[j - 1] > key) {
            _divisors[j] = _divisors[j - 1];
            --j;
        }
        _divisors[j] = key;
    }

    _divisors_ready = true;
}

seq_t a(idx_t n)
{
    if (! _divisors_ready)
        _init_divisors();

    if (n < 1 || n > 12)
        return (seq_t)0;   /* undefined – return 0 for out‑of‑range */

    return _divisors[(size_t)(n - 1)];
}
