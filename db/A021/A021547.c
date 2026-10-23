/*
 * A021547  Decimal expansion of 1/543.
 * https://oeis.org/A021547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = nth decimal digit of 1/543 (digit extraction of the
 *            decimal expansion)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* seq_t and idx_t are defined by the build system */

static bool _a547_inited = false;
static seq_t *_a547_digits = NULL;
static size_t _a547_period = 0;

static void _a547_init(void) {
    if (_a547_inited) return;
    const idx_t denom = 543;          /* denominator of the fraction */
    /* The period of 1/543 divides φ(543)=360; allocate that many slots */
    seq_t *buf = (seq_t *)malloc(360 * sizeof(seq_t));
    if (!buf) return;                 /* allocation failed – ignore cleanup */

    idx_t rem = 1;
    size_t i = 0;
    do {
        rem = rem * 10;                /* shift remainder by one decimal place */
        buf[i] = rem / denom;          /* digit */
        rem = rem % denom;             /* new remainder */
        i++;
    } while (rem != 1);                /* repeat until we return to the start */

    _a547_digits = buf;
    _a547_period = i;
    _a547_inited = true;
}

seq_t a(idx_t n) {
    _a547_init();
    size_t pos = (size_t)(n % _a547_period);
    return _a547_digits[pos];
}
