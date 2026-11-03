/*
 * A023388  a(n+1) = a(n) converted to base 8 from base 7 (written in base
 *          10).
 * https://oeis.org/A023388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 7; for n>=1, a(n+1) = convert the base‑7 representation
 *            of a(n) into base‑8 (read the digits as an octal number)
 * Range:     u8 n <= 25, u16 n <= 38, u32 n <= 50, u64 n <= 60, u128 n <= 71,
 *            big any n
 * Validated: standard, pass, 63 terms through n = 120 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Convert a value written in decimal to its base‑7 digit string,
   then interpret that digit string as base‑8 and return the
   corresponding decimal value.  All arithmetic is performed in
   seq_t. */
static inline seq_t convert_base8_from_base7(seq_t x) {
    /* Buffer large enough for the base‑7 digits of values up to
       ~10^580 (≈700 digits).  2048 covers well beyond that range. */
    uint8_t digits[2048];
    idx_t di = 0;

    /* Extract base‑7 digits, least‑significant first. */
    while (x > (seq_t)0) {
        seq_t rem = x % (seq_t)7;
        digits[di++] = (uint8_t)rem;
        x = x / (seq_t)7;
    }
    if (di == 0) {          /* x was 0 */
        digits[0] = 0;
        di = 1;
    }

    /* Reinterpret the digit array as base‑8. */
    seq_t res = (seq_t)0;
    for (idx_t j = di; j-- > 0;) {
        res = res * (seq_t)8 + (seq_t)digits[j];
    }
    return res;
}

/* Main sequence function.  Returns a(n) for n>=1. */
seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0; /* Undefined for offset reasons, but safe. */

    seq_t val = (seq_t)7;       /* a(1) */
    for (idx_t i = 1; i < n; ++i) {
        val = convert_base8_from_base7(val);
    }
    return val;
}
