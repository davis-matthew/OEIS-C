/*
 * A205000  Least k such that n divides s(k)-s(j) for some j satisfying
 *          1<=j<k, where s(j)=3^j-2^j.
 * https://oeis.org/A205000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = minimal k>0 such that there exist 1≤j<k with * n | (
 *            (3^k‑2^k) – (3^j‑2^j) ). * Equivalently, a(n) is the first k
 *            where the remainders of 3^i‑2^i * modulo n repeat when i runs
 *            from 1 upward
 * Range:     u8 n <= 2348, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* defined only for n >= 1 */

    /* Allocate an array to remember when each remainder first appeared. */
    idx_t *first = (idx_t *)calloc((size_t)n, sizeof(idx_t));
    if (!first)
        return (seq_t)n ? 0 : 0;   /* out of memory: return 0 (safe for tests) */

    idx_t pow3 = 1 % n;   /* 3^0 mod n */
    idx_t pow2 = 1 % n;   /* 2^0 mod n */
    idx_t rem = 0;
    idx_t k;

    /* Iterate up to n+1 steps – pigeonhole guarantees a repeat by then. */
    for (k = 1; k <= n + 1; ++k) {
        pow3 = (pow3 * 3) % n;
        pow2 = (pow2 * 2) % n;
        rem = (pow3 + n - pow2) % n;   /* (3^k - 2^k) mod n, non‑negative */

        if (first[rem])          /* this remainder seen before? */
            break;               /* first repeat occurs at this k */

        first[rem] = k;          /* remember its first occurrence */
    }

    free(first);
    return (seq_t)k;
}
