/*
 * A005128  Number of k for which n does not divide Stirling_2 subset numbers
 *          S(n, k).
 * https://oeis.org/A005128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{k : S(n,k) mod n != 0}, where S(i,k) = k*S(i-1,k) +
 *            S(i-1,k-1) mod n, S(0,0) = 1 (a(0) = 1)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 1;

    unsigned long long m = (unsigned long long)n;
    unsigned long long *row = (unsigned long long *)calloc((size_t)n + 1, sizeof(unsigned long long));
    if (!row) return 0;

    /* row holds S(i, k) mod n; start with i = 0 */
    row[0] = 1 % m;
    for (idx_t i = 1; i <= n; i++) {
        for (idx_t k = i; k >= 1; k--) {
            unsigned long long v = ((unsigned long long)k % m) * row[k] % m + row[k - 1];
            if (v >= m) v -= m;
            row[k] = v;
        }
        row[0] = 0;
    }

    seq_t count = 0;
    for (idx_t k = 0; k <= n; k++) {
        if (row[k] != 0) count++;
    }

    free(row);
    return count;
}
