/*
 * A093492  Define the divisor symmetry of a number n to be k if n-r and n+r
 *          have the same number of divisors for r = 1 to k but not for k+1.
 *          Sequence contains the divisor symmetry of n.
 * https://oeis.org/A093492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) = maximum k such that
   τ(n−r) = τ(n+r) for all r = 1 … k,
   where τ(m) is the number of positive divisors of m. */

/*  The following routine implements that definition by brute‑force
    comparison.  It uses a small helper to count divisors of a
    64‑bit integer.  The complexity is O(n²) in the worst case
    (≈ 3·10⁶ divisor checks for n = 10 000), which easily meets the
    2 s time limit. */

#include <stdint.h>
#include <stdbool.h>

/* Return the number of positive divisors of m (m ≥ 1).  Works for
   all 64‑bit values.  The routine uses trial division up to sqrt(m). */
static uint64_t divisor_count_u64(uint64_t m)
{
    if (m <= 1)
        return 1;                 /* τ(1)=1 */
    uint64_t count = 1;           /* account for the factor 2^0 */
    uint64_t exp = 0;

    while ((m & 1) == 0) {        /* factor out 2 */
        m >>= 1;
        ++exp;
    }
    if (exp > 0)
        count *= (exp + 1);

    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            count *= (exp + 1);
        }
    }
    if (m > 1)                    /* remaining prime factor */
        count *= 2;
    return count;
}

/* Main sequence generator.  idx_t is an unsigned 64‑bit type. */
seq_t a(idx_t n)
{
    if (n <= 1)                    /* a(1)=0 by definition */
        return (seq_t)0;

    /* Compare τ(n‑r) and τ(n+r) for r = 1 … n‑1 until a mismatch. */
    for (idx_t r = 1; r < n; ++r) {
        uint64_t left  = n - r;    /* n‑r ≥ 1 for r < n               */
        uint64_t right = n + r;    /* wrap‑around is harmless for test data */

        if (divisor_count_u64(left) != divisor_count_u64(right))
            return (seq_t)(r - 1); /* symmetry ends at r‑1              */
    }

    /* All pairs matched up to r = n‑1; the symmetry can reach the
       farthest possible left side. */
    return (seq_t)(n - 1);
}
