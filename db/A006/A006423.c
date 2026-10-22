/*
 * A006423  Number of rooted toroidal maps with 3 faces and n vertices and
 *          without separating cycles or isthmuses.
 * https://oeis.org/A006423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+1)*(n+2)*(n+3)*(29*n^5 + 762*n^4 + 5111*n^3 +
 *            7902*n^2 + 8*n + 5088)/45360, computed as binomial(n+3,4) * P(n)
 *            / 1890 with the factors of 1890 cancelled before multiplying
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 23, u64 n <= 309, u128 n <= 1000,
 *            big any n
 * Helpers:   seq_is_fixed_width, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"
#include "../utils/u128_to_seq.h"

static const unsigned primes1890[6] = {2, 3, 3, 3, 5, 7};

static unsigned __int128 a_u128(uint64_t n) {
    unsigned __int128 x = n;
    unsigned __int128 b = x;
    unsigned __int128 p, d = 1;
    int i;
    b = b * (x + 1) / 2;
    b = b * (x + 2) / 3;
    b = b * (x + 3) / 4;
    p = ((((29 * x + 762) * x + 5111) * x + 7902) * x + 8) * x + 5088;
    for (i = 0; i < 6; i++) {
        if (b % primes1890[i] == 0) b /= primes1890[i];
        else d *= primes1890[i];
    }
    p /= d;
    return b * p;
}

static seq_t a_big(idx_t n) {
    seq_t x = (seq_t)n;
    seq_t b = x;
    seq_t p;
    seq_t d = 1;
    int i;
    b = b * (x + 1) / 2;
    b = b * (x + 2) / 3;
    b = b * (x + 3) / 4;
    p = ((((29 * x + 762) * x + 5111) * x + 7902) * x + 8) * x + 5088;
    for (i = 0; i < 6; i++) {
        if (b % primes1890[i] == 0) b /= primes1890[i];
        else d *= primes1890[i];
    }
    p /= d;
    return b * p;
}

seq_t a(idx_t n) {
    if (seq_is_fixed_width())
        return u128_to_seq(a_u128((uint64_t)n));
    return a_big(n);
}
