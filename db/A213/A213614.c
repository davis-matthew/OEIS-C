/*
 * A213614  Decimal expansion of the length of one light-year in meters.
 * https://oeis.org/A213614
 *
 * Offset:    16    (a(16) is the first term)
 * Keywords:  nonn, cons, full, fini
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A213614: decimal digit of 9460730472580800 at position n (offset 16) */
#include <stdint.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Offsets below 16 are undefined; return 0 for safety */
    if (n < 16)
        return 0;

    /* 0‑based index into the 16‑digit sequence */
    idx_t pos = n - 16;
    if (pos >= 16)  /* beyond the known digits */
        return 0;

    /* Light‑year length in meters (fits in 64‑bit) */
    const uint64_t LY = 9460730472580800ULL;

    /* Compute power of 10 for the desired digit position */
    idx_t k = 15 - pos;         /* 0 for the last digit, 15 for the first */
    seq_t pow10k = pow10(k);    /* pow10 returns seq_t, but 10^15 fits in 64‑bit */
    uint64_t pow_val = (uint64_t)pow10k;

    uint64_t digit = (LY / pow_val) % 10;
    return (seq_t)digit;
}
