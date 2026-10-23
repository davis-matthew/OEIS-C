/*
 * A021531  Decimal expansion of 1/527.
 * https://oeis.org/A021531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = decimal digit of 1/527 at position n (starting after the
 *            decimal point)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *digits = NULL;
static idx_t period = 0;
static bool initialized = false;

static void init_digits(void) {
    if (initialized) return;

    const uint64_t denom = 527ull;
    uint64_t r = 1ull % denom;          /* initial remainder */
    /* 1/527 has period 240 (known). Allocate a temporary buffer long enough for the cycle. */
    const idx_t temp_cap = 240;
    seq_t *tmp = (seq_t*)malloc(temp_cap * sizeof(seq_t));
    idx_t len = 0;

    do {
        r *= 10ull;
        uint64_t d = r / denom;        /* digit 0‑9 */
        r = r % denom;                 /* next remainder */
        tmp[len++] = (seq_t)d;
    } while (r != 1ull);               /* stop when the cycle restarts */

    period = len;
    digits = (seq_t*)malloc(period * sizeof(seq_t));
    for (idx_t i = 0; i < period; ++i) digits[i] = tmp[i];

    free(tmp);
    initialized = true;
}

seq_t a(idx_t n) {
    if (!initialized) init_digits();
    idx_t pos = n % period;            /* repetition of the period */
    return digits[pos];
}
