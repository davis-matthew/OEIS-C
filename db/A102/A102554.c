/*
 * A102554  Numbers k such that p <> (k AND p) for at least one prime-factor
 *          p.
 * https://oeis.org/A102554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A102554 by brute‑force search */
/* Numbers k such that ∃ prime factor p of k with (k & p) ≠ p */

#include <stdbool.h>
#include <stdint.h>

/* Return true iff k has a prime factor p with (k & p) != p.  */
static bool good_k(uint64_t k)
{
    uint64_t temp = k;

    /* Handle factor 2 */
    if ((temp & 1ULL) == 0) {
        if ((k & 2ULL) != 2ULL)
            return true;
        while ((temp & 1ULL) == 0)
            temp >>= 1;
    }

    /* Check odd prime factors */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            if ((k & p) != p)
                return true;
            while (temp % p == 0)
                temp /= p;
        }
    }

    /* Remaining prime factor, if any */
    if (temp > 1) {
        if ((k & temp) != temp)
            return true;
    }

    return false;
}

/* Return the n‑th term of A102554 (a(1) = 4).  */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* offset is 1; this value is unused */

    idx_t  count = 0;
    uint64_t k = 2;        /* start checking from 2 */

    while (count < n) {
        if (good_k(k)) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
        ++k;
    }

    /* Should never reach here for valid n */
    return (seq_t)0;
}
