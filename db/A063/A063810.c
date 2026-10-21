/*
 * A063810  Growth series for Heisenberg group.
 * https://oeis.org/A063810
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = (c_n + 31*n^3 - 57*n^2 + 105*n) / 18 where the 12‑term
 *            periodic constant c_n is defined for n ≥ 1 as: c_1 = -7, c_2 =
 *            -14, c_3 = 9, c_4 = -16, c_5 = -23, c_6 = 18, c_7 = -7, c_8 =
 *            -32, c_9 = 9, c_10 = 2, c_11 = -23, c_12 = 0, and c_{n+12} = c_n
 *            for all n ≥ 1. Additionally, a(0) = 1
 * Range:     u8 n <= 5, u16 n <= 34, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <stdint.h>   /* for uint64_t */

seq_t a(idx_t n)
{
    /* Special case for offset 0 */
    if (n == 0) return (seq_t)1;

    /* Predefined periodic c_n values */
    static const int c[12] = {
        -7, -14, 9, -16, -23, 18,
        -7, -32, 9, 2, -23, 0
    };

    /* Convert n to signed and unsigned forms for calculations */
    unsigned long long un = (unsigned long long)n;      /* unsigned version for modulus */
    long long ll_n = (long long)n;                      /* signed for arithmetic */

    /* Determine which c_n to use (n ≥ 1, so index = (n-1) mod 12) */
    size_t idx = (size_t)((un - 1) % 12);
    int cn = c[idx];

    /* Compute numerator using 64‑bit arithmetic (safe for n up to ~1e6) */
    long long num = 31LL * ll_n * ll_n * ll_n
                    - 57LL * ll_n * ll_n
                    + 105LL * ll_n
                    + (long long)cn;

    /* Divides evenly by 18, result fits in seq_t for the intended ranges */
    return (seq_t)(num / 18LL);
}
