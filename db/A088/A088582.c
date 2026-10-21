/*
 * A088582  a(n) = n*x^n + (n-1)*x^(n-1) + . . . + x + 1 for x=4.
 * https://oeis.org/A088582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (13 + (3*n-1)*4^(n+1))/9
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=5, a(2)=37, a(n)=9*a(n-1)-24*a(n-2)+16*a(n-3) for n>=3 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n == 0ULL) return (seq_t)1;
    if (n == 1ULL) return (seq_t)5;
    if (n == 2ULL) return (seq_t)37;

    /* Keep only the three most recent values to avoid dynamic allocation. */
    seq_t a0 = (seq_t)1;   /* a(n-3) */
    seq_t a1 = (seq_t)5;   /* a(n-2) */
    seq_t a2 = (seq_t)37;  /* a(n-1) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = ((seq_t)9) * a2 - ((seq_t)24) * a1 + ((seq_t)16) * a0;
        a0 = a1;
        a1 = a2;
        a2 = cur;
    }
    return a2;
}
