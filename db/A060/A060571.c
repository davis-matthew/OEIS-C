/*
 * A060571  Tower of Hanoi: the optimal way to move an even number of disks
 *          from peg 0 to peg 2 or an odd number from peg 0 to peg 1 is on
 *          move n to move disk A001511 from peg A060571 (here) to peg
 *          A060572.
 * https://oeis.org/A060571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0 if n ≤ 2 or n is a power of two a(n) = ( a(n – 2^⌊log₂
 *            n⌋) – (–1)^⌊log₂ n⌋ ) mod 3 otherwise (the same recursion that
 *            generates A060571)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External linkage: required function */
seq_t a(idx_t n);

#include <stdint.h>

/* Helper: recursive computation of a(n) */
static seq_t _a_recursive(idx_t n)
{
    /* Base cases: for n = 1, 2 (and for powers of two) the value is 0 */
    if (n <= 2)
        return 0;

    /* Find k = ⌊log₂ n⌋ and pow = 2^k (largest power of two ≤ n) */
    idx_t pow = 1;
    int k = 0;
    while ((pow << 1) <= n) {
        pow <<= 1;
        ++k;
    }

    /* If n is exactly a power of two, a(n) = 0 */
    if (n == pow)
        return 0;

    /* General case: n > pow */
    seq_t prev = _a_recursive(n - pow);
    int sign = (k & 1) ? -1 : 1;            /* (–1)^k  */
    int res  = (int)prev - sign;            /* a(prev) – (–1)^k  */
    res %= 3;                                /* modulo 3  */
    if (res < 0)
        res += 3;
    return (seq_t)res;
}

/* Public function as required by the build */
seq_t a(idx_t n)
{
    return _a_recursive(n);
}
