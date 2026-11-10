/*
 * A097144  Primes not appearing in A097143.
 * https://oeis.org/A097144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- a(n) is the n-th prime not in A097143, where
 *            A097143(1) = 2 and A097143(k) = least prime p > A097143(k-1)
 *            with Jacobi(k, p) = 1. Both are walked together, one prime at a
 *            time
 * Range:     u8 n <= 23, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-07)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

/* smallest prime strictly greater than x */
static uint64_t A097144_next_prime_gt(uint64_t x)
{
    if (x < 2) return 2;
    uint64_t c = x + 1;
    if (c > 2 && (c & 1) == 0) c++;
    while (!is_prime_u64(c)) c += 2;
    return c;
}

static int A097144_jacobi(uint64_t a, uint64_t n) {
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t m = n & 7;
            if (m == 3 || m == 5) res = -res;
        }
        uint64_t tmp = a; a = n; n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3) res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    uint64_t t = 2;      /* current term of A097143 */
    uint64_t k = 1;      /* its index */
    uint64_t q = 2;      /* current prime being examined */
    idx_t count = 0;
    for (;;) {
        if (q == t) {
            k++;
            uint64_t p = A097144_next_prime_gt(t);
            while (A097144_jacobi(k, p) != 1) p = A097144_next_prime_gt(p);
            t = p;
        } else {
            count++;
            if (count == n) return (seq_t)q;
        }
        q = A097144_next_prime_gt(q);
    }
}
