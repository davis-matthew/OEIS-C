/*
 * A108154  a(n) = n^2 - n + 1 if prime else 0.
 * https://oeis.org/A108154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*n - n + 1 if prime, else 0
 * Range:     u8 n <= 17, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t val_idx = n * n - n + 1;
    return is_prime((unsigned long long)val_idx) ? (seq_t)val_idx : (seq_t)0;
}
