/*
 * A166085  Number of times Sum_{i=1..u} J(i,4n+1) obtains value zero when u
 *          ranges from 1 to (4n+1). Here J(i,k) is the Jacobi symbol.
 * https://oeis.org/A166085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 150, u16 n <= 8277, u32 n <= 8277, u64 n <= 8277,
 *            u128 n <= 8277, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = Number of u in [1,4n+1] where the partial sum of Jacobi symbols
   J(i,4n+1) equals zero (i = 1..u). */

#include <stdint.h>

static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    int result = 1;
    while (a != 0) {
        while ((a & 1ULL) == 0ULL) {
            a >>= 1ULL;
            uint64_t n_mod_8 = n & 7ULL;
            if (n_mod_8 == 3ULL || n_mod_8 == 5ULL)
                result = -result;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3ULL) == 3ULL && (n & 3ULL) == 3ULL)
            result = -result;
        a %= n;
    }
    return (n == 1ULL) ? result : 0;
}

seq_t a(idx_t n) {
    uint64_t k = 4ULL * (uint64_t)n + 1ULL;   /* modulus 4n+1 */
    int sum = 0;                              /* running sum of J(i,k) */
    uint64_t zeros = 0;                       /* count of zero sums */

    for (uint64_t i = 1ULL; i < k; ++i) {
        int js = jacobi_symbol(i, k);
        sum += js;
        if (sum == 0) ++zeros;
    }
    return (seq_t)zeros;
}
