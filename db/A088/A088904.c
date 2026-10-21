/*
 * A088904  Number of ways to write n = x^i + x^j with 1<x<=n and 0<=i<=j.
 * https://oeis.org/A088904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A088904
    Formula: a(n) = |{ (x,i,j) : 1 < x ≤ n, 0 ≤ i ≤ j, n = x^i + x^j }|  */
#include <stdint.h>

/* external types supplied by the build */
extern seq_t a(idx_t n) {
    seq_t count = 0;
    if (n < 2)                /* no base x can satisfy 1 < x ≤ n */
        return count;

    const seq_t sn = (seq_t)n;          /* cast once for speed */
    seq_t powers[128];                  /* enough for any base up to 2^64 */

    for (idx_t x = 2; x <= n; ++x) {
        /* generate all powers of x that are ≤ n */
        seq_t pow = 1;          /* x^0 */
        int m = 0;
        while (pow <= sn) {
            powers[m++] = pow;
            if (pow > sn / (seq_t)x)        /* next multiplication would overflow n */
                break;
            pow = pow * (seq_t)x;
        }

        /* count unordered pairs (i,j) with i ≤ j that sum to n */
        for (int i = 0; i < m; ++i) {
            for (int j = i; j < m; ++j) {
                seq_t sum = powers[i] + powers[j];
                if (sum == sn)
                    ++count;
            }
        }
    }
    return count;
}
