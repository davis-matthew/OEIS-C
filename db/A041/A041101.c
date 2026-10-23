/*
 * A041101  Denominators of continued fraction convergents to sqrt(58).
 * https://oeis.org/A041101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 198*a(n-7) + a(n-14) for n >= 14. Initial terms: a(0)=1,
 *            a(1)=1, a(2)=2, a(3)=3, a(4)=5, a(5)=8, a(6)=13, a(7)=190,
 *            a(8)=203, a(9)=393, a(10)=596, a(11)=989, a(12)=1585, a(13)=2574
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 30, u64 n <= 60, u128 n <= 118,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base values for n = 0 .. 13. */
    static const seq_t base[14] = {
        1ULL, 1ULL, 2ULL, 3ULL, 5ULL, 8ULL, 13ULL,
        190ULL, 203ULL, 393ULL, 596ULL, 989ULL, 1585ULL, 2574ULL
    };

    /* For n < 14, the answer is found directly in the table. */
    if (n < 14)
        return base[n];

    /* Allocate an array big enough to hold all terms up to n. */
    const idx_t len = n + 1;
    seq_t *terms = (seq_t *)malloc((size_t)len * sizeof(seq_t));
    if (terms == NULL)
        return 0;             /* Out of memory – return zero as a safe fallback. */

    /* Copy the initial 14 terms. */
    for (idx_t i = 0; i < 14; ++i)
        terms[i] = base[i];

    /* Compute the remainder of the sequence using the recurrence. */
    for (idx_t i = 14; i <= n; ++i)
        terms[i] = (seq_t)198 * terms[i - 7] + terms[i - 14];

    const seq_t result = terms[n];
    free(terms);
    return result;
}
