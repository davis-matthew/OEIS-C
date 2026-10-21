/*
 * A070776  Numbers k such that number of terms in the k-th cyclotomic
 *          polynomial is equal to the largest prime factor of k.
 * https://oeis.org/A070776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 150, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A070776
   Return the nth integer k (n ≥ 1) such that the number of terms in the
   k‑th cyclotomic polynomial equals the largest prime factor of k.
   This is exactly the set of integers > 1 whose odd part is a power of a
   single odd prime (including the pure powers of 2). */

/* To compute the sequence we simply iterate through integers, test
   the property above, and count until we reach the requested index. */

#include <stdint.h>
#include <stdbool.h>

/* Helper: test whether n satisfies the property. */
static bool is_special(uint64_t n)
{
    if (n <= 1) return false;          /* exclude 1 */

    /* Remove all factors of 2 – we only need to inspect the odd part. */
    uint64_t m = n;
    while ((m & 1ULL) == 0) m >>= 1;

    /* Pure powers of 2 are always valid. */
    if (m == 1) return true;

    /* Find an odd prime divisor of m. */
    uint64_t p = 0;
    uint64_t tmp = m;
    for (uint64_t d = 3; d <= tmp / d; d += 2) {
        if (tmp % d == 0) {
            p = d;
            break;
        }
    }
    if (p == 0) p = tmp;               /* m itself is prime */

    /* Divide out all copies of p; if anything remains, m was not a
       pure power of a single odd prime. */
    while (m % p == 0) m /= p;
    return (m == 1);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    uint64_t candidate = 2;           /* the first admissible integer */

    while (true) {
        if (is_special(candidate)) {
            count++;
            if (count == n) {
                return (seq_t)candidate;
            }
        }
        candidate++;
    }
}
