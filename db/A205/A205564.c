/*
 * A205564  Least positive integer j such that n divides 2k!-2j!, where k, as
 *          in A205563, is the least number for which there is such a j.
 * https://oeis.org/A205564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS sequence A205564.
 * We let m = n / (n % 2 ? 1 : 2).  Then n | 2k! - 2j! ⇔ k! ≡ j! (mod m)
 * with j < k.  The function finds the smallest k for which the modular
 * factorial r(k) repeats, and returns the first occurrence index j of that
 * residue, which is the desired a(n).  The algorithm runs in O(m) time
 * and O(m) space, where m ≤ n. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Reduce n by removing a factor of two if present. */
    idx_t m = n / ((n & 1ULL) ? 1ULL : 2ULL);

    /* Allocate array mapping a residue to its first occurrence index. */
    idx_t *first = (idx_t *)calloc((size_t)m, sizeof(idx_t));
    if (!first) {
        /* Allocation failure – return 0 (undefined behaviour is avoided). */
        return (seq_t)0;
    }

    idx_t r = 1 % m;          /* r = 1! mod m for k = 1 */
    first[r] = 1;             /* record first occurrence */

    for (idx_t k = 2; ; ++k) {
        /* r = k! mod m, using 128‑bit arithmetic to avoid overflow. */
        r = (idx_t)(((__uint128_t)r * k) % m);

        if (first[r] != 0) {          /* repeat found */
            idx_t j = first[r];
            free(first);
            return (seq_t)j;
        }
        first[r] = k;                  /* record first occurrence of this residue */
    }
}
