/*
 * A175594  Numbers having no primitive root.
 * https://oeis.org/A175594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = nth number that has no primitive root.  a(1) = 0.  */

#include <stdint.h>
#include <stdbool.h>

/* Return true iff x is a power of an odd prime.  */
static bool is_odd_prime_power_uint64(uint64_t x)
{
    if (x < 3)                 /* 1,2 are not odd prime powers */
        return false;
    if ((x & 1ULL) == 0)       /* even numbers are not odd prime powers */
        return false;

    uint64_t p = 0;
    for (uint64_t d = 3; d <= x / d; d += 2)   /* trial division to sqrt(x) */
    {
        if (x % d == 0)
        {
            p = d;              /* found a divisor; it is the required odd prime */
            break;
        }
    }

    if (p == 0)                /* x itself is an odd prime */
        return true;

    /* Divide out all powers of p */
    while (x % p == 0)
        x /= p;
    return x == 1;             /* true iff x was a pure power of p */
}

/* Primitive roots exist only for n = 1, 2, 4, p^k, or 2·p^k with odd prime p.  */
static bool has_primitive_root_uint64(uint64_t n)
{
    if (n == 1 || n == 2 || n == 4)
        return true;

    if ((n & 1ULL) == 0)       /* even */
        return is_odd_prime_power_uint64(n >> 1);  /* check n/2 */
    else
        return is_odd_prime_power_uint64(n);      /* odd n */
}

/* OEIS sequence A175594 */
seq_t a(idx_t n)
{
    if (n == 1)                 /* a(1) = 0 */
        return (seq_t)0;

    idx_t target = n - 1;       /* number of values with no primitive root we need */
    uint64_t count = 0;         /* how many such numbers seen so far */
    uint64_t x = 2;             /* start scanning from 2 upwards */

    while (count < target)
    {
        if (!has_primitive_root_uint64(x))
        {
            ++count;
            if (count == target)
                return (seq_t)x;
        }
        ++x;
    }

    /* Should never be reached */
    return (seq_t)0;
}
