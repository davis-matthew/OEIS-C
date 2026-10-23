/*
 * A174980  Stern's diatomic series type ([0,1], 1).
 * https://oeis.org/A174980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf, look
 * Formula:   a(0)=0; a(2n+1)=a(n); a(2n)=a(n-1)+a(n)+[n is power of 2]
 * Range:     u8 n <= 8190, u16 n <= 8190, u32 n <= 8190, u64 n <= 8190,
 *            u128 n <= 8190, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the recurrence directly, with a simple power‑of‑two test. */
#include <stdint.h>
#include <stdbool.h>

/* Helper: test whether v is a power of two (v > 0). */
static inline bool is_power_of_two(idx_t v)
{
    return v && ((v & (v - 1)) == 0);
}

/* Compute the A174980 term at index n. */
seq_t a(idx_t n)
{
    /* base case */
    if (n == 0)
        return (seq_t)0;

    /* odd index: a(2m+1) = a(m) */
    if (n & 1U)
        return a(n >> 1);

    /* even index: a(2m) = a(m-1) + a(m) + [m is power of 2] */
    idx_t m = n >> 1;                    /* m = n / 2 */
    seq_t term = a(m - 1) + a(m);        /* recursive terms */
    if (is_power_of_two(m))
        term = term + (seq_t)1;           /* add 1 if m is power of 2 */

    return term;
}
