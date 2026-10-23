/*
 * A182999  Decimal expansion of c^2, c being the speed of light in vacuum in
 *          SI units.
 * https://oeis.org/A182999
 *
 * Offset:    17    (a(17) is the first term)
 * Keywords:  cons, nonn, fini, full
 * Formula:   a(n) = (C2 / 10^(33 - n)) % 10, where C2 = 89875517873681764,
 *            for n >= 17; a(n) = 0 for n > 33
 * Range:     u8 n <= 33, u16 n <= 33, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Number whose decimal digits are needed */
    const uint64_t C2 = 89875517873681764ULL;

    /* Precomputed 10^k for k = 0..16 (fits in 64-bit) */
    static const uint64_t POW10[17] = {
        1ULL, 10ULL, 100ULL, 1000ULL, 10000ULL, 100000ULL, 1000000ULL,
        10000000ULL, 100000000ULL, 1000000000ULL, 10000000000ULL,
        100000000000ULL, 1000000000000ULL, 10000000000000ULL,
        100000000000000ULL, 1000000000000000ULL, 10000000000000000ULL
    };

    /* The sequence is defined only for n ≥ 17; beyond the digits of C2 we
       return 0.  The number C2 has 17 decimal digits, so a(33) is the
       least‑significant digit. */
    if (n < 17) return 0;
    if (n > 33) return 0;

    idx_t exp = 33 - n;            /* exponent of 10 */
    uint64_t digit = (C2 / POW10[exp]) % 10ULL;
    return (seq_t)digit;
}
