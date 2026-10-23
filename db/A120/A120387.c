/*
 * A120387  c(n) mod b(n) where c(n) = (n-1)! and b(n) = Sum_{i=1..n} i.
 * https://oeis.org/A120387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0 if n and n+1 are composite; otherwise a(n) = n*(n-1)/2
 *            for even n, and a(n) = n*(n-1)/2 - 1 for odd n
 * Range:     u8 n <= 27, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    if (n == 1)
        return 0;

    bool prime_n = is_prime(n);
    /* guard against overflow when n is the maximum uint64_t value */
    bool prime_next;
    if (n == (idx_t)-1)
        prime_next = false;
    else
        prime_next = is_prime(n + 1);

    if (!prime_n && !prime_next)
        return 0;

    seq_t val = (seq_t)n * (seq_t)(n - 1) / 2;
    if (n % 2 == 1)   /* odd */
        return val - 1;
    else              /* even */
        return val;
}
