/*
 * A037830  Number of i such that d(i) >= d(i-1), where Sum_{i=0..m} d(i)*7^i
 *          is the base-7 representation of n.
 * https://oeis.org/A037830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = |{ i ≥ 1 : d(i) ≥ d(i−1) }| where the digits d(i) are the
 *            base‑7 digits of n (least significant digit d(0) )
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t count = 0;
    if (n == 0) return count;          /* defined for n ≥ 1, handled for safety */

    uint64_t prev = n % 7;              /* least significant digit d(0) */
    n /= 7;

    while (n > 0) {
        uint64_t curr = n % 7;          /* next digit d(i) */
        if (curr >= prev)
            count++;                    /* satisfy d(i) ≥ d(i-1) */
        prev = curr;
        n /= 7;
    }
    return count;
}
