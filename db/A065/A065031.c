/*
 * A065031  In the decimal expansion of n, replace each odd digit with 1 and
 *          each even digit with 2.
 * https://oeis.org/A065031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = replace each odd decimal digit of n with 1 and each even
 *            digit with 2
 * Range:     u8 n <= 999, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case for n = 0: “0” → 2 */
    if (n == 0)
        return (seq_t)2;

    /* Pre‑compute powers of ten up to 10^19 (fits in 64‑bit). */
    static const uint64_t pow10[20] = {
        1ULL, 10ULL, 100ULL, 1000ULL, 10000ULL, 100000ULL,
        1000000ULL, 10000000ULL, 100000000ULL, 1000000000ULL,
        10000000000ULL, 100000000000ULL, 1000000000000ULL,
        10000000000000ULL, 100000000000000ULL, 1000000000000000ULL,
        10000000000000000ULL, 100000000000000000ULL, 1000000000000000000ULL,
        10000000000000000000ULL
    };

    /* Find the most significant digit position (0‑based). */
    idx_t pos = 0;
    for (idx_t i = 0; i < 20; ++i) {
        if (pow10[i] <= (uint64_t)n)
            pos = i;
        else
            break;
    }

    seq_t result = 0;

    /* Process digits from most significant to least significant. */
    for (idx_t i = pos; ; --i) {
        unsigned digit = (unsigned)((n / pow10[i]) % 10ULL);
        unsigned mapped = (digit & 1U) ? 1U : 2U;
        result = result * 10 + (seq_t)mapped;
        if (i == 0)
            break;
    }

    return result;
}
