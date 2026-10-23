/*
 * A147648  Number of distinct even superperfect numbers dividing n.
 * https://oeis.org/A147648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of even superperfect numbers dividing n An even
 *            superperfect number is 2^k with k+1 a Mersenne prime’s exponent;
 *            such numbers are 2, 4, 16, 64, 4096, 65536, 262144, 1073741824,
 *            1152921504606846976,
 * Range:     u8 n <= 8717, u16 n <= 8717, u32 n <= 8717, u64 n <= 8717,
 *            u128 n <= 8717, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Element types seq_t and idx_t are supplied externally */
#include <stddef.h>   /* for size_t */

static const idx_t superperfect_vals[] = {
    ((idx_t)1 << 1),   /* 2      */
    ((idx_t)1 << 2),   /* 4      */
    ((idx_t)1 << 4),   /* 16     */
    ((idx_t)1 << 6),   /* 64     */
    ((idx_t)1 << 12),  /* 4096   */
    ((idx_t)1 << 16),  /* 65536  */
    ((idx_t)1 << 18),  /* 262144 */
    ((idx_t)1 << 30),  /* 1073741824   */
    ((idx_t)1 << 60)   /* 1152921504606846976 */
};

seq_t a(idx_t n)
{
    /* Count even superperfect divisors of n */
    const size_t count = sizeof(superperfect_vals) / sizeof(superperfect_vals[0]);
    seq_t res = 0;
    for (size_t i = 0; i < count; ++i) {
        if (superperfect_vals[i] > n)
            break;
        if (n % superperfect_vals[i] == 0)
            ++res;
    }
    return res;
}
