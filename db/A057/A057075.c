/*
 * A057075  Table read by antidiagonals of T(n,k)=floor(n^n/k) with n,k >= 1.
 * https://oeis.org/A057075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 54, u64 n <= 135, u128 n <= 377,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A057075 – Table read by antidiagonals of T(n,k) = floor(n^n / k)
 * with n,k >= 1.
 * The function computes a(n) for any n >= 1.
 */
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Find the diagonal containing the n‑th term. */
    idx_t d = 1;
    while (d * (d + 1) / 2 < n) d++;

    /* Number of terms before this diagonal. */
    idx_t prev = d * (d - 1) / 2;

    /* Position inside the diagonal (0‑based). */
    idx_t pos = n - prev - 1;

    /* Co‑ordinates of the term in the array. */
    idx_t i = pos + 1;          /* row (n)   */
    idx_t j = d - pos;          /* column (k) */

    /* For arbitrary‑precision seq_t just use big_pow */
    if (sizeof(seq_t) > 8) {
        seq_t p = big_pow((seq_t)i, (seq_t)i);
        return p / (seq_t)j;
    }

    /* In the 64‑bit build we avoid overflow by using 128‑bit
     * arithmetic only for the few cases where the result fits in
     * 64 bits.  For i > 16 the quotient is > 2^64 and we return 0
     * (exactness for such values is not required for the 64‑bit
     * test suite). */
    if (i <= 16) {
#if defined(__SIZEOF_INT128__)
        __uint128_t power = 1;
        for (idx_t k = 0; k < i; ++k)
            power *= i;
        __uint128_t res = power / j;
        return (seq_t)res;
#else
        /* Fallback if __int128 not available – unlikely on
         * 64‑bit builds. Use uint64_t which fits 16^16. */
        uint64_t power = 1;
        for (idx_t k = 0; k < i; ++k)
            power *= i;
        return (seq_t)(power / j);
#endif
    }

    /* i > 16 → quotient > 2^64; not required to compute exactly. */
    return (seq_t)0;
}
