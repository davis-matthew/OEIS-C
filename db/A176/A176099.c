/*
 * A176099  Prime(n) + A158611(n).
 * https://oeis.org/A176099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n) + A158611(n) * where A158611(1)=0, A158611(2)=1,
 *            and for n≥3, A158611(n)=prime(n-2)
 * Range:     u8 n <= 32, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    if (n == 1) return 2;          /* prime(1)+0 = 2+0 */
    if (n == 2) return 4;          /* prime(2)+1 = 3+1 */
    /* For n ≥ 3, a(n) = prime(n) + prime(n-2) */
    seq_t p1 = (seq_t)get_prime(n);
    seq_t p2 = (seq_t)get_prime(n - 2);
    return p1 + p2;
}
