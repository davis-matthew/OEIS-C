/*
 * A018342  Divisors of 225.
 * https://oeis.org/A018342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 225 (3²·5²) in increasing order */
#include <stdlib.h>
#include <stdbool.h>

static seq_t _divs[9];
static bool _divs_initialized = false;

static int cmp_seq(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

static void init_divisors(void)
{
    if (_divs_initialized) return;

    size_t pos = 0;
    for (int e3 = 0; e3 <= 2; ++e3) {
        seq_t p3 = 1;
        for (int i = 0; i < e3; ++i) p3 *= (seq_t)3;
        for (int e5 = 0; e5 <= 2; ++e5) {
            seq_t p5 = 1;
            for (int i = 0; i < e5; ++i) p5 *= (seq_t)5;
            _divs[pos++] = p3 * p5;
        }
    }
    qsort(_divs, 9, sizeof(seq_t), cmp_seq);
    _divs_initialized = true;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined, but safe */
    if (n > 9) return (seq_t)0;          /* out of bounds */

    init_divisors();
    return _divs[(size_t)(n - 1)];
}
