/*
 * A011579  Stirling numbers of second kind S2(30,n).
 * https://oeis.org/A011579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = Stirling number of the second kind S(30,n) (the number of
 *            ways to partition a 30‑element set into n non‑empty subsets)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool _a_init = false;
static seq_t _a_cache[31];  // indices 1..30 store the values

static void _a_compute(void)
{
    seq_t prev[31] = {0};
    seq_t curr[31] = {0};

    prev[0] = 1;  // S(0,0) = 1

    for (idx_t i = 1; i <= 30; ++i) {
        curr[0] = 0;
        for (idx_t k = 1; k <= i; ++k) {
            curr[k] = k * prev[k] + prev[k - 1];
        }
        for (idx_t k = 0; k <= i; ++k) {
            prev[k] = curr[k];
        }
    }

    for (idx_t k = 1; k <= 30; ++k) {
        _a_cache[k] = prev[k];
    }

    _a_init = true;
}

seq_t a(idx_t n)
{
    if (!_a_init) {
        _a_compute();
    }

    if (n >= 1 && n <= 30) {
        return _a_cache[n];
    }
    return (seq_t)0;
}
