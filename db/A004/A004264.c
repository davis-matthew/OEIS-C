/*
 * A004264  a(n) = ceiling(100*log_2(n)).
 * https://oeis.org/A004264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceiling(100*log_2(n)) = number of bits of n^100 - 1
 *            (exact multiprecision computation of n^100)
 * Range:     u8 n <= 5, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    uint64_t L[110];
    size_t len = 1, j;
    int i;
    uint64_t bits;
    L[0] = 1;
    for (i = 0; i < 100; i++) {
        unsigned __int128 carry = 0;
        for (j = 0; j < len; j++) {
            unsigned __int128 t = (unsigned __int128)L[j] * n + carry;
            L[j] = (uint64_t)t;
            carry = t >> 64;
        }
        if (carry) L[len++] = (uint64_t)carry;
    }
    /* subtract 1 (n^100 >= 1) */
    j = 0;
    while (L[j] == 0) { L[j] = ~(uint64_t)0; j++; }
    L[j]--;
    while (len > 0 && L[len - 1] == 0) len--;
    if (len == 0) return (seq_t)0;
    bits = (uint64_t)(len - 1) * 64 + (uint64_t)(64 - __builtin_clzll(L[len - 1]));
    return (seq_t)bits;
}
