/*
 * A174813  a(n) = number whose product of digits equals a power of 3.
 * https://oeis.org/A174813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 282, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumeration: a(n) = the nth positive integer whose decimal
   digits are all from the set {1,3,9}, listed in increasing order. */

/* C11 / C++17 compatible implementation. */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;               /* undefined for n<1, return 0 safely */

    /* Find the digit length d of the n-th term. */
    idx_t pow3 = 1;            /* will become 3^d after each increment   */
    idx_t cumulative = 0;      /* count of numbers with <d digits       */
    idx_t d = 0;

    while (1) {
        pow3 *= (idx_t)3;       /* 3^d */
        ++d;
        if (cumulative + pow3 >= n)
            break;
        cumulative += pow3;
    }

    /* 0‑based rank of the term within the group of d‑digit numbers. */
    idx_t rank = n - cumulative - 1;

    /* Convert the rank to base‑3 digits with exactly d digits. */
    int digits[64];            /* 64 > max d for all 64‑bit n (≈41)       */
    for (idx_t pos = d; pos > 0; --pos) {
        digits[pos - 1] = rank % (idx_t)3;
        rank /= (idx_t)3;
    }

    /* Map base‑3 digits 0,1,2 to decimal digits 1,3,9. */
    static const seq_t map[3] = { (seq_t)1, (seq_t)3, (seq_t)9 };

    seq_t value = 0;
    for (idx_t pos = 0; pos < d; ++pos)
        value = value * (seq_t)10 + map[digits[pos]];

    return value;
}
