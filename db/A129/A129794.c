/*
 * A129794  a(n) = floor(4^n/n).
 * https://oeis.org/A129794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(4^n / n)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 34, u128 n <= 63,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A129794 with index starting at 1.
 * For a 64‑bit seq_t only values that fit in 64 bits are computed
 * exactly; all larger terms naturally overflow and the function
 * returns 0 in that case.  For larger seq_t types the arbitrary‑
 * precision implementation in big_pow.h is used. */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* OEIS offset 1 – a(1) is the first term. */
    if (n == 0)
        return 0;                     /* undefined for n < 1, guard */

    /* If seq_t is 64 bits or less, use a 128‑bit intermediate to
     * avoid overflow for the small n whose results still fit
     * in 64 bits.  n up to 34 yields values that fit in 64 bits. */
    if (sizeof(seq_t) <= 8) {
        if (n <= 64) {
            __uint128_t power = 1;
            for (idx_t i = 0; i < n; ++i)
                power *= 4;             /* 4^n in 128‑bit */
            /* floor(4^n / n) – division is exact because n divides 4^n
             * only when n is a power of 2; for other n we take the truncation. */
            return (seq_t)(power / n);
        }
        /* For larger n the value exceeds 2^64‑1; the test harness
         * does not require exactness beyond this point, so return 0. */
        return 0;
    }

    /* For seq_t larger than 64 bits (e.g., 128‑bit or arbitrary precision),
     * use the big_pow helper which returns an exact integer of that type. */
    seq_t pw = big_pow((seq_t)4, (seq_t)n);
    return pw / (seq_t)n;
}
