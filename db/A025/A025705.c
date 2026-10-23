/*
 * A025705  Index of 4^n within the sequence of the numbers of the form
 *          4^i*10^j.
 * https://oeis.org/A025705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + n + \sum_{k=1}^{n} \lfloor k\,\log_{10}(4) \rfloor
 * Range:     u8 n <= 27, u16 n <= 465, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Numerator and denominator for the rational approximation of log10(4)
   \log_{10}(4) \approx 602059991327962390 / 1000000000000000000.  */
static const uint64_t LOG10_4_NUM = 602059991327962390ULL;
static const uint64_t LOG10_4_DEN = 1000000000000000000ULL;

/* Return floor(k * log10(4)) using integer arithmetic.
   The product fits in 128 bits for all practically relevant k (≤10⁶). */
static inline seq_t floor_kc(idx_t k)
{
    return (seq_t)(((__int128)k * LOG10_4_NUM) / LOG10_4_DEN);
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
        sum += floor_kc(k);
    return 1 + (seq_t)n + sum;
}
