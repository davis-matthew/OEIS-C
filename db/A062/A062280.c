/*
 * A062280  a(1) = 1; a(n) = smallest multiple of n-th prime (n>1) with all
 *          odd digits.
 * https://oeis.org/A062280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A062280: a(1)=1;
   for n>1, a(n)=smallest multiple of the n-th prime
   such that all decimal digits are odd */
#include <stdbool.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;

    uint32_t prim = get_prime(n);          /* nth prime (1-indexed) */
    idx_t k = 1;
    while (1) {
        seq_t val = (seq_t)prim * (seq_t)k;
        seq_t tmp = val;
        bool odd = true;
        while (tmp > 0) {
            seq_t d = tmp % 10;
            if ((d & 1) == 0) {            /* even digit found */
                odd = false;
                break;
            }
            tmp /= 10;
        }
        if (odd)
            return val;
        ++k;
    }
}
