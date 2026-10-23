/*
 * A040838  Continued fraction for sqrt(868).
 * https://oeis.org/A040838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   Continued fraction for sqrt(868). a(0)=floor(sqrt(868)), then
 *            a(n) for n≥1 repeats the period 2,6,19,2,19,6,2,58
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static bool _a040838_inited = false;
static seq_t _a040838_a0 = 0;
static seq_t _a040838_period[8];
static size_t _a040838_period_len = 0;

static void _a040838_init(void) {
    if (_a040838_inited) return;
    /* compute floor(sqrt(868)) */
    uint64_t a0_int = 0;
    while ((a0_int + 1) * (a0_int + 1) <= 868ULL) a0_int++;
    _a040838_a0 = (seq_t)a0_int;

    /* continued fraction expansion of sqrt(868) */
    uint64_t m = 0, d = 1, a = a0_int;
    do {
        m = d * a - m;
        d = (868ULL - m * m) / d;
        a = (a0_int + m) / d;
        _a040838_period[_a040838_period_len++] = (seq_t)a;
    } while (a != 2 * a0_int); /* period ends when a == 2*a0 */

    _a040838_inited = true;
}

seq_t a(idx_t n) {
    _a040838_init();
    if (n == 0) return _a040838_a0;
    size_t idx = (size_t)(n - 1) % _a040838_period_len;
    return _a040838_period[idx];
}
