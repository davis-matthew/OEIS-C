/*
 * A182337  List of positive integers whose prime tower factorization, as
 *          defined in comments, does not contain the prime 3.
 * https://oeis.org/A182337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 156, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   good_exponent
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A182337 – numbers whose prime‐tower factorization contains no prime 3.
 *  The function a(n) returns the nth term (a(1)=1).
 */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/good_exponent.h"

static inline bool is_good(uint64_t n)
{
    if (n % 3 == 0) return false;          /* base prime 3 at the top level */
    if (n == 1) return true;                 /* 1 has empty factorisation */
    uint64_t t = n;
    for (uint64_t p = 2; p * p <= t; ++p)
    {
        if (t % p == 0)
        {
            uint64_t exp = 0;
            while (t % p == 0)
            {
                t /= p;
                ++exp;
            }
            if (p == 3) return false;        /* base prime 3 in factorisation */
            if (!good_exponent(exp)) return false;
        }
    }
    if (t > 1) /* remaining prime factor */
    {
        // remaining prime is > sqrt(n); exponent is 1
        if (t == 3) return false;            /* base prime 3 in factorisation */
        // exponent 1 is always good
    }
    return true;
}

seq_t a(idx_t n)
{
    idx_t cnt = 0;
    uint64_t v = 0;
    for (uint64_t k = 1; ; ++k)
    {
        if (is_good(k))
        {
            ++cnt;
            if (cnt == n)
            {
                v = k;
                break;
            }
        }
    }
    return (seq_t)v;
}
