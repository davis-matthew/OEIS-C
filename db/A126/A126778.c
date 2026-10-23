/*
 * A126778  Number of functions f:{1,2,...,n}->{1,2,...,n} such that Im(f)
 *          contain two fixed elements.
 * https://oeis.org/A126778
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^n - 2*(n-1)^n + (n-2)^n, n >= 2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include <stdint.h>

/* Helper: compute base^exp using 128-bit arithmetic (safe for seq_t <= 64 bits) */
static unsigned __int128 pow128(unsigned __int128 base, idx_t exp) {
    unsigned __int128 result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t result;
    /* If seq_t fits in 64 bits, use 128-bit temporary to avoid overflow */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 term1 = pow128((unsigned __int128)n, n);
        unsigned __int128 term2 = pow128((unsigned __int128)(n > 0 ? n - 1 : 0), n);
        unsigned __int128 term3 = pow128((unsigned __int128)(n > 1 ? n - 2 : 0), n);
        unsigned __int128 temp = term1 - 2 * term2 + term3;
        result = (seq_t)temp;
    } else {
        seq_t nn = (seq_t)n;
        seq_t n1 = (seq_t)(n > 0 ? n - 1 : 0);
        seq_t n2 = (seq_t)(n > 1 ? n - 2 : 0);
        seq_t term1 = big_pow(nn, nn);
        seq_t term2 = big_pow(n1, nn);
        seq_t term3 = big_pow(n2, nn);
        result = term1 - 2 * term2 + term3;
    }
    return result;
}
