/*
 * A100574  If n = product{p|n, p=prime} p^b(p,n), where each b(p,n) is a
 *          positive integer and the product is over distinct prime divisors
 *          of n, a(n) = difference between the maximum p^b(p,n) and minimum
 *          p^b(p,n).
 * https://oeis.org/A100574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = difference between the maximum and minimum prime‑power *
 *            factors of n: a(n) = max{p^b(p,n)} - min{p^b(p,n)} * where n = ∏
 *            p^b(p,n). * Implementation: trial division factorization of n;
 *            for each prime p * compute p^b and track the extreme values. For
 *            primes or powers of a * single prime the...
 * Range:     u8 n <= 525, u16 n <= 9886, u32 n <= 9886, u64 n <= 9886,
 *            u128 n <= 9886, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   // may be optional – for standard integer types

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                       // a(1) = 0

    idx_t temp = n;
    seq_t min = 0, max = 0;
    int first = 1;                      // flag for first prime factor found

    /* Factor 2 separately */
    if ((temp & 1) == 0) {
        seq_t pow = 1;
        while ((temp & 1) == 0) {
            pow *= (seq_t)2;
            temp >>= 1;
        }
        if (first) {
            min = max = pow;
            first = 0;
        } else {
            if (pow < min) min = pow;
            if (pow > max) max = pow;
        }
    }

    /* Factor odd primes */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            seq_t pow = 1;
            while (temp % p == 0) {
                pow *= (seq_t)p;
                temp /= p;
            }
            if (first) {
                min = max = pow;
                first = 0;
            } else {
                if (pow < min) min = pow;
                if (pow > max) max = pow;
            }
        }
    }

    /* Remaining prime factor > sqrt(original n) */
    if (temp > 1) {
        seq_t pow = (seq_t)temp;
        if (first) {
            min = max = pow;
        } else {
            if (pow < min) min = pow;
            if (pow > max) max = pow;
        }
    }

    return max - min;
}
