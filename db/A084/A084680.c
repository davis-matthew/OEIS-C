/*
 * A084680  Order of 10 modulo n [i.e., least m such that 10^m = 1 (mod n)] or
 *          0 when no such number exists.
 * https://oeis.org/A084680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the multiplicative order of 10 modulo n.
   For n not coprime to 10 (i.e. divisible by 2 or 5) the value is 0.
   For n == 1 the value is 1 by definition.  The order is found by
   repeated multiplication by 10 modulo n. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    /* If 10 and n are not coprime, the order does not exist. */
    if ((n & 1) == 0 || (n % 5) == 0)
        return (seq_t)0;

    seq_t curr = (seq_t)(10ULL % n);  /* 10^1 mod n */
    idx_t m = 1;

    while (curr != (seq_t)1) {
        ++m;
        /* Use 64‑bit intermediate to avoid overflow in small seq_t types */
        uint64_t prod = (uint64_t)curr * 10ULL;
        curr = (seq_t)(prod % (uint64_t)n);
    }

    return (seq_t)m;
}
