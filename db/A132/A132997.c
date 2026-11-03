/*
 * A132997  Divisors of 299792458, the speed of light (meters/second).
 * https://oeis.org/A132997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, easy, less
 * Formula:   list the divisors of 299792458 in ascending order
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Validated: full, pass, 16 terms through n = 16 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static void _compare_seq_t(const void *p1, const void *p2, void *out)
{
    seq_t a = *((const seq_t *)p1);
    seq_t b = *((const seq_t *)p2);
    if (a < b) *((int *)out) = -1;
    else if (a > b) *((int *)out) = 1;
    else *((int *)out) = 0;
}

static int _seq_t_compare(const void *p1, const void *p2)
{
    seq_t a = *((const seq_t *)p1);
    seq_t b = *((const seq_t *)p2);
    return (a > b) - (a < b);
}

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
        mem_qsort(_divs, 16, sizeof(seq_t), _seq_t_compare);
        _ready = true;
    }

    if (n == 0 || n > 16) return (seq_t)0;
    return _divs[n - 1];
}
