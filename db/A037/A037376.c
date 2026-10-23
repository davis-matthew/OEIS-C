/*
 * A037376  Positive numbers k such that every base-2 digit of k is a base-7
 *          digit of k.
 * https://oeis.org/A037376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A037376: n-th positive integer k such that every base-2 digit of k
   appears as a base-7 digit of k.
   This implementation searches k in increasing order and tests the
   digit condition.  The algorithm runs in O(n) time with a small
   constant factor. */

#include <stdbool.h>

static bool passes(seq_t k)
{
    /* Check whether the binary representation of k contains a 0 bit.
       It does iff k is not of the form 2^m-1.  This is equivalent to
       (k+1) & k != 0. */
    bool hasZeroBinary = ((k + 1) & k) != 0;

    /* Scan base‑7 digits; we only care whether the digits 0 and 1 appear. */
    seq_t t = k;
    bool hasZero7 = false;
    bool hasOne7  = false;
    while (t > (seq_t)0) {
        unsigned int digit = (unsigned int)(t % (seq_t)7);
        if (digit == 0) hasZero7 = true;
        else if (digit == 1) hasOne7 = true;
        t = t / (seq_t)7;
    }
    if (!hasOne7) return false;          /* digit 1 must be present */
    if (hasZeroBinary && !hasZero7) return false; /* digit 0 also required */
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for n=0, return 0 as placeholder */
    idx_t count = 0;
    seq_t k = 1;
    while (true) {
        if (passes(k)) {
            ++count;
            if (count == n) return k;
        }
        ++k;
    }
}
