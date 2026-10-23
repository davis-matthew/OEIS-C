/*
 * A140644  a(0)=1. a(n) = a(n-1)/d(n) if d(n) divides a(n-1). Otherwise, a(n)
 *          = a(n-1)*d(n). (d(n) is the number of positive divisors of n.).
 * https://oeis.org/A140644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 215, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; a(n)=a(n-1)/d(n) if d(n) divides a(n-1), else a(n)=a(n-1)*d(n),
   where d(n) is the number of positive divisors of n. */
#include <stdint.h>

/* Count positive divisors of n. */
static inline seq_t divisor_count(idx_t n)
{
    seq_t cnt = (seq_t)0;
    if (n == 0) return (seq_t)0;
    for (idx_t j = 1; j * j <= n; ++j) {
        if (n % j == 0) {
            if (j * j == n) {
                ++cnt;
            } else {
                cnt += (seq_t)2;
            }
        }
    }
    return cnt;
}

/* OEIS A140644 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t prev = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t d = divisor_count(i);
        if (prev % d == 0) {
            prev /= d;
        } else {
            prev *= d;
        }
    }
    return prev;
}
