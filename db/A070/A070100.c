/*
 * A070100  Number of integer triangles with perimeter n and prime side
 *          lengths which are acute and isosceles.
 * https://oeis.org/A070100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts acute isosceles integer triangles with perimeter n
 *            and all three prime side lengths
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Simple trial–division primality test for numbers up to n (≤ 10000). */
static bool isPrime(idx_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (idx_t i = 3; i * i <= x; i += 2)
        if (x % i == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n < 3) return 0;

    seq_t count = 0;

    /* Equilateral (p,p,p) case: 3p = n */
    if (n % 3 == 0) {
        idx_t p = n / 3;
        if (isPrime(p))
            count++;
    }

    /* Isosceles with two equal sides p,p and third side q = n-2p */
    idx_t max_p = (n - 1) / 2;           /* ensures q > 0 */
    for (idx_t p = 2; p <= max_p; ++p) {
        if (!isPrime(p)) continue;
        idx_t q = n - 2 * p;
        if (q <= 0) continue;
        if (!isPrime(q)) continue;
        if (p == q) continue;            /* already counted as equilateral */

        /* Triangle inequality: 2p > q */
        if (2 * p <= q) continue;

        /* Acute condition: only needed when q > p (q is longest side) */
        if (q > p) {
            uint64_t lhs = (uint64_t)2 * (uint64_t)p * (uint64_t)p;
            uint64_t rhs = (uint64_t)q * (uint64_t)q;
            if (lhs <= rhs) continue;   /* not acute */
        }

        count++;
    }

    return count;
}
