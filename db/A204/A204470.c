/*
 * A204470  Number of 6-element subsets that can be chosen from {1,2,...,6*n}
 *          having element sum 18*n+3.
 * https://oeis.org/A204470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence a(0)=0, a(1)=1, a(2)=58,
 *            a(3)=676, a(4)=3486, a(5)=11963, a(6)=32134, a(7)=73294,
 *            a(8)=148718, a(9)=276373, a(10)=479632. For n >= 11: a(n) =
 *            4*a(n-1) -5*a(n-2) +0*a(n-3) +5*a(n-4) -3*a(n-5) -3*a(n-6)
 *            +5*a(n-7) +0*a(n-8) -5*a(n-9) +4*a(n-10) -1*a(n-11) (c[n] =...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 59, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const int coeff[11] = {4,-5,0,5,-3,-3,5,0,-5,4,-1};

static const seq_t init[11] = {
    0ULL, 1ULL, 58ULL, 676ULL, 3486ULL, 11963ULL,
    32134ULL, 73294ULL, 148718ULL, 276373ULL, 479632ULL
};

seq_t a(idx_t n) {
    if (n < 11) return init[n];

    seq_t buf[11];
    for (size_t i = 0; i < 11; ++i) buf[i] = init[i];

    for (idx_t i = 11; i <= n; ++i) {
        seq_t pos = 0, neg = 0;
        for (int k = 1; k <= 11; ++k) {
            size_t idx = (size_t)(i - k) % 11;
            seq_t prev = buf[idx];
            int c = coeff[k-1];
            if (c > 0) {
                pos += prev * (unsigned int)c;
            } else if (c < 0) {
                neg += prev * (unsigned int)(-c);
            }
        }
        seq_t next = pos - neg;          /* pos >= neg by construction */
        buf[(size_t)i % 11] = next;
    }
    return buf[(size_t)n % 11];
}
