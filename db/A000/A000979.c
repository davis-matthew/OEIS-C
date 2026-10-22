/*
 * A000979  Wagstaff primes: primes of form (2^p + 1)/3.
 * https://oeis.org/A000979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^p + 1)/3 where p = A000978(n), the n-th exponent
 *            making (2^p+1)/3 prime (exponents from a table of known values;
 *            finding them requires a large primality search)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint64_t a000979_exps[] = {
    3, 5, 7, 11, 13, 17, 19, 23, 31, 43, 61, 79, 101, 127, 167, 191, 199,
    313, 347, 701, 1709, 2617, 3539, 5807, 10501, 10691, 11279, 12391,
    14479, 42737, 83339, 95369, 117239, 127031, 138937, 141079, 267017,
    269987, 374321, 986191, 4031399, 13347311, 13372531
};

seq_t a(idx_t n) {
    idx_t count = (idx_t)(sizeof(a000979_exps) / sizeof(a000979_exps[0]));
    if (n < 1 || n > count) return 0;
    uint64_t p = a000979_exps[n - 1];
    seq_t result = (seq_t)1;
    seq_t base = (seq_t)2;
    while (p > 0) {
        if (p & 1) result = result * base;
        p >>= 1;
        if (p > 0) base = base * base;
    }
    return (result + 1) / 3;
}
