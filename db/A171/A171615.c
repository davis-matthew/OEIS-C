/*
 * A171615  Numbers n with property that (n^2 + sum of the digits of n^2) is
 *          odd.
 * https://oeis.org/A171615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers n such that n^2 + digit_sum(n^2) is odd */

#include <stdint.h>
#include <stdbool.h>

/* seq_t and idx_t are defined by the build system */
extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    idx_t count = 0ULL;
    idx_t cand = 1ULL;

    while (true) {
        /* Compute n^2 in 128‑bit arithmetic to avoid overflow. */
        unsigned __int128 sq = (unsigned __int128)cand * cand;

        /* Sum the decimal digits of n^2. */
        unsigned long long sum = 0ULL;
        unsigned __int128 tmp = sq;
        while (tmp > 0ULL) {
            sum += (unsigned long long)(tmp % 10ULL);
            tmp /= 10ULL;
        }

        /* Check the parity condition. */
        if (((cand + sum) & 1ULL) == 1ULL) {
            ++count;
            if (count == n)
                return (seq_t)cand;
        }
        ++cand;
    }
}
