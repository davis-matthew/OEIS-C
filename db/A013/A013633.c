/*
 * A013633  a(n) = nextprime(n) - prevprime(n).
 * https://oeis.org/A013633
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nextprime(n) - prevprime(n)
 * Range:     u8 n <= 2000, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 3) return 0;             /* offset is 3; behaviour below is undefined */
    idx_t prev = n - 1;
    while (prev >= 2 && !is_prime(prev)) {
        --prev;
    }
    idx_t next = n + 1;
    while (!is_prime(next)) {
        ++next;
    }
    return (seq_t)next - (seq_t)prev;
}
