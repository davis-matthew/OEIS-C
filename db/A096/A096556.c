/*
 * A096556  Consecutive internal states of the linear congruential
 *          pseudo-random number generator used in the Watcom FORTRAN Function
 *          URAND when started with SEED=0.
 * https://oeis.org/A096556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2147437301 * a(n-1) + 453816693) mod 2^31 with a(1) = 0.
 *            The function works for any seq_t type (unsigned integer or
 *            arbitrary‑precision class) and any idx_t type (typically
 *            uint64_t)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)                 /* safety: offset is 1, but return 0 for 0 */
        return (seq_t)0;

    const uint64_t multiplier = 2147437301ULL; /* 0x8000000 + 31…1? */
    const uint64_t adder      = 453816693ULL;
    const uint64_t mask32     = 0x7FFFFFFFULL;   /* 2^31-1 */

    seq_t cur = (seq_t)0;       /* a(1) */

    /* If n==1, return cur immediately. */
    if (n == 1)
        return cur;

    /* Iterate until the desired term is reached. */
    for (idx_t i = 2; i <= n; ++i)
    {
        /* Convert cur to a 32‑bit value for the multiplication. */
        uint32_t cur32 = (uint32_t)cur;
        uint64_t prod = (uint64_t)cur32 * multiplier + adder;
        cur = (seq_t)(prod & mask32);   /* modulo 2^31 */
    }
    return cur;
}
