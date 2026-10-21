/*
 * A069966  (Sum of digits of n)^6 - (sum of digits^6 of n).
 * https://oeis.org/A069966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = (sum_of_digits(n))^6 - sum_of_digits^6(n)
 * Range:     u8 n <= 11, u16 n <= 15, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Pre‑computed d^6 for digits 0–9 (unsigned long long to avoid overflow in initialisation) */
    static const unsigned long long pow6[10] = {
        0ULL,      /* 0^6 */
        1ULL,      /* 1^6 */
        64ULL,     /* 2^6 */
        729ULL,    /* 3^6 */
        4096ULL,   /* 4^6 */
        15625ULL,  /* 5^6 */
        46656ULL,  /* 6^6 */
        117649ULL, /* 7^6 */
        262144ULL, /* 8^6 */
        531441ULL  /* 9^6 */
    };

    unsigned long long s = 0;   /* sum of decimal digits of n */
    unsigned long long t = 0;   /* sum of each digit^6 */

    idx_t tmp = n;
    while (tmp) {
        unsigned d = (unsigned)(tmp % 10ULL);
        s += d;
        t += pow6[d];
        tmp /= 10ULL;
    }

    /* Compute s^6 carefully in unsigned long long
       (s ≤ 180 for n up to 2^64‑1, so s^6 ≤ 3.4×10^13, fits in 64 bits) */
    unsigned long long s6 = 1;
    for (int i = 0; i < 6; ++i) {
        s6 *= s;
    }

    return (seq_t)(s6 - t);
}
