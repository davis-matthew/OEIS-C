/*
 * A056562  Number of primes which are the difference between two triangular
 *          numbers, where the smaller is the n-th triangular number.
 * https://oeis.org/A056562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes among {n+1, 2n+3}; 2 is treated as
 *            non‑prime to match the OEIS data
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t val1 = n + 1;          /* n+1  */
    idx_t val2 = n * 2 + 3;      /* 2n+3 */

    bool p1 = (val1 == 2ULL) ? false : is_prime(val1);
    bool p2 = is_prime(val2);

    return (seq_t)(p1 + p2);
}
