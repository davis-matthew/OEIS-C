/*
 * A130891  a(n) = n if n is not an odd prime number. Otherwise, a(n) =
 *          k*floor(n/10), where k is the smallest integer such that n < 10^k.
 * https://oeis.org/A130891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A130891:
 *  a(n) = n  if n is not an odd prime number;
 *  otherwise a(n) = k * floor(n/10), where k is the smallest integer such that
 *  n < 10^k (i.e. the number of decimal digits of n). */
#include <stdbool.h>
#include "../utils/is_prime.h"

/* count decimal digits of a 64‑bit integer (works for idx_t which is uint64_t) */
static inline int digit_count_idx(idx_t n)
{
    int d = 0;
    do {
        n /= 10;
        d++;
    } while (n);
    return d;
}

seq_t a(idx_t n)
{
    /* If n is not an odd prime return n itself. */
    if (!(n % 2 == 1 && is_prime(n))) {
        return (seq_t)n;
    }

    /* n is an odd prime */
    int d = digit_count_idx(n);
    seq_t tens   = (seq_t)(n / 10);       /* floor(n/10)  */
    seq_t result = (seq_t)d * tens;       /* k * floor(n/10) */
    return result;
}
