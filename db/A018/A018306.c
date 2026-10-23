/*
 * A018306  Divisors of 152.
 * https://oeis.org/A018306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 152, sorted ascending. Computation by enumerating all divisors of 152. */
#include <stdbool.h>

static bool _divs_init = false;
static seq_t _divs[8];

static void _init_divs(void)
{
    uint64_t n = 152;
    idx_t cnt = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            _divs[cnt++] = (seq_t)i;
            if (i * i != n) {
                _divs[cnt++] = (seq_t)(n / i);
            }
        }
    }
    /* sort ascending */
    for (idx_t p = 0; p < 8 - 1; ++p) {
        for (idx_t q = p + 1; q < 8; ++q) {
            if (_divs[p] > _divs[q]) {
                seq_t t = _divs[p];
                _divs[p] = _divs[q];
                _divs[q] = t;
            }
        }
    }
    _divs_init = true;
}

seq_t a(idx_t n)
{
    if (!_divs_init) _init_divs();
    if (n >= 1 && n <= 8) {
        return _divs[n - 1];
    }
    return 0; /* undefined for n > 8 */
}
