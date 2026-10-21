/*
 * A092445  a(n) is the first term of the sexy prime quadruple a(n), a(n)+6,
 *          a(n)+12 and a(n)+18 that becomes a perfect square if the rightmost
 *          digit (1) is removed.
 * https://oeis.org/A092445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A092445: a(n) is the first term p of a “sexy prime quadruple”
 * (p, p+6, p+12, p+18) such that
 *      p ≡ 1 (mod 10)   and   p/10 is a perfect square
 * (i.e. p = 10·s²+1 for some integer s).
 * The routine simply iterates over s, tests the four primalities,
 * and counts the valid quadruples.  When the counter reaches n,
 * the corresponding p is returned.  This yields all terms in
 * increasing order without any hard‑coded table.  The code compiles
 * for any seq_t (uint8_t … unsigned __int128) and idx_t (uint64_t).
 */

#include <stdbool.h>
#include "../utils/isPrime.h"

seq_t a(idx_t n)
{
    idx_t count = 0;  /* how many valid quadruples we have found so far */
    idx_t s     = 0;  /* current integer s, p = 10·s²+1 */

    while (true) {
        ++s;
        /* compute p = 10*s^2 + 1 using 64‑bit arithmetic (safe for all n considered) */
        unsigned long long val = 10ULL * s * s + 1ULL;
        seq_t p = (seq_t)val;

        /* The four numbers of the quadruple */
        if (!isPrime(val))            continue;
        if (!isPrime(val + 6ULL))     continue;
        if (!isPrime(val + 12ULL))    continue;
        if (!isPrime(val + 18ULL))    continue;

        ++count;
        if (count == n)
            return p;
    }
}
