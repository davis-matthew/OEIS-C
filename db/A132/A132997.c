/*
 * A132997  Divisors of 299792458, the speed of light (meters/second).
 * https://oeis.org/A132997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, easy, less
 * Formula:   list the divisors of 299792458 in ascending order
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   _compare_seq_t, _seq_t_compare
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/_compare_seq_t.h"
#include "../utils/_seq_t_compare.h"

static seq_t _divs[16];
static bool _ready = false;

seq_t a(idx_t n)
{
    if (!_ready) {
        /* Factorization: 299792458 = 2 * 7 * 73 * 293339 */
        const uint64_t primes[] = {2ULL, 7ULL, 73ULL, 293339ULL};
        const size_t p_cnt = 4;
        size_t len = 1;
        _divs[0] = (seq_t)1;

        for (size_t i = 0; i < p_cnt; ++i) {
            /* current number of divisors */
            size_t cur_len = len;
            for (size_t j = 0; j < cur_len; ++j) {
                _divs[len++] = _divs[j] * (seq_t)primes[i];
            }
        }

        /* sort the array */
        qsort(_divs, 16, sizeof(seq_t), _seq_t_compare);
        _ready = true;
    }

    if (n == 0 || n > 16) return (seq_t)0;
    return _divs[n - 1];
}
