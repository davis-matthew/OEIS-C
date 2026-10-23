/*
 * A136333  Numbers containing only digits coprime to 10 in their decimal
 *          representation.
 * https://oeis.org/A136333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   generates the n-th integer containing only the digits 1,3,7,9.
 *            The sequence is obtained by enumerating numbers with k digits,
 *            where k is the minimum such that the cumulative count of numbers
 *            with fewer than k digits is less than n. The offset within the
 *            k‑digit block is represented in base‑4 (dig...
 * Range:     u8 n <= 36, u16 n <= 852, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    static const unsigned char map[4] = {1, 3, 7, 9};
    __uint128_t sum = 0;       // total numbers with fewer than k digits
    __uint128_t pow4 = 4;      // 4^k
    unsigned k = 1;            // current number of digits

    while (sum + pow4 < (__uint128_t)n) {
        sum += pow4;
        pow4 *= 4;
        ++k;
    }

    uint64_t offset = (uint64_t)n - (uint64_t)sum;   // 1 … 4^k
    uint64_t off = offset - 1;                       // zero‑based inside block

    unsigned char rev[64];                          // reversed base‑4 digits
    for (unsigned i = 0; i < k; ++i) {
        rev[i] = (unsigned char)(off % 4);
        off /= 4;
    }

    seq_t result = 0;
    for (int i = (int)k - 1; i >= 0; --i) {
        result = result * 10 + map[rev[i]];
    }

    return result;
}
