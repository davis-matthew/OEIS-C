/*
 * A053143  Smallest square divisible by n.
 * https://oeis.org/A053143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, mult
 * Formula:   a(n) = smallest square divisible by n → For each prime p
 *            dividing n, multiply by p if its exponent in n is odd
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* behavior below offset is irrelevant */
    seq_t res = (seq_t)n;
    uint64_t tmp = n;

    /* handle factor 2 */
    if ((tmp & 1) == 0) {
        unsigned e = 0;
        while ((tmp & 1) == 0) { tmp >>= 1; e++; }
        if (e & 1) res = res * (seq_t)2;
    }

    /* odd prime factors */
    for (uint64_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            unsigned e = 0;
            while (tmp % p == 0) { tmp /= p; e++; }
            if (e & 1) res = res * (seq_t)p;
        }
    }

    /* remaining prime factor (exponent 1) */
    if (tmp > 1) res = res * (seq_t)tmp;

    return res;
}
