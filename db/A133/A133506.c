/*
 * A133506  Numbers n such that powertrain(n) < n.
 * https://oeis.org/A133506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A133506: numbers n such that powertrain(n) < n
   (powertrain is defined by starting with the most significant digit
   and repeatedly raising to the next digit).
   This implementation tests each candidate until the n‑th term is found. */
#include <stdint.h>
#include <stdbool.h>

/* Return true if the powertrain of n is strictly less than n. */
static bool powertrain_lt(idx_t n)
{
    if (n < 10) return false;   /* one‑digit numbers never satisfy */

    /* Extract decimal digits of n (least significant first). */
    uint8_t digits[64];
    int len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        digits[len++] = tmp % 10;
        tmp /= 10;
    }

    /* Compute powertrain using an unsigned 128‑bit accumulator to avoid
       intermediate overflow when the result exceeds 64 bits. */
    __uint128_t power = digits[len - 1];          /* most significant digit */
    const __uint128_t limit = (unsigned __int128)n;

    for (int i = len - 2; i >= 0; --i) {
        unsigned d = digits[i];
        if (d == 0) {
            power = 1;           /* any number to the power of 0 is 1 */
            continue;
        }
        __uint128_t prod = 1;
        for (unsigned j = 0; j < d; ++j) {
            prod *= power;
            if (prod > limit)    /* no need to continue once it exceeds n */
                return false;
        }
        power = prod;
    }
    return power < limit;
}

/* Return the n‑th term of A133506 (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, guard against overflow */

    idx_t count = 0;               /* how many qualifying numbers found */
    idx_t candidate = 1;           /* start checking from 1 upward */

    while (count < n) {
        ++candidate;
        if (powertrain_lt(candidate))
            ++count;
    }

    return (seq_t)candidate;
}
