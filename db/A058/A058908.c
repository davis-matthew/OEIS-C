/*
 * A058908  Six prime numbers in arithmetic progression with a common
 *          difference of 9876543210.
 * https://oeis.org/A058908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = 5077 + (n-1)*9876543210
 * Range:     u8 none, u16 n <= 1, u32 n <= 1, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A058908 – six primes in AP with common difference 9876543210. */

#include <stdint.h>

static const seq_t A1 = 5077;
static const seq_t D  = 9876543210ULL;

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset starts at 1 */
    seq_t nm1 = (seq_t)(n - 1);                /* (n-1) as seq_t */
    return A1 + nm1 * D;                      /* A1 + (n-1)*D */
}
