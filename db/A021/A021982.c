/*
 * A021982  Decimal expansion of 1/978.
 * https://oeis.org/A021982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/978) mod 10 the sequence is the decimal
 *            expansion of 1/978
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

static uint8_t digits[2000];
static int seen[978];
static size_t period_start = 0;
static size_t period_len = 0;
static int initialized = 0;

static void init(void) {
    if (initialized) return;
    size_t pos = 0;
    int r = 1;
    while (seen[r] == 0 && pos < sizeof(digits)) {
        seen[r] = (int)(pos + 1);         /* store position+1 to distinguish unset */
        int digit = (int)((r * 10) / 978);
        digits[pos++] = (uint8_t)digit;
        r = (int)((r * 10) % 978);
    }
    period_start = (size_t)(seen[r] - 1);
    period_len   = pos - period_start;
    initialized = 1;
}

seq_t a(idx_t n) {
    init();
    if (n < period_start) {
        return (seq_t)digits[(size_t)n];
    } else {
        size_t offset = (size_t)((n - period_start) % period_len);
        return (seq_t)digits[period_start + offset];
    }
}
