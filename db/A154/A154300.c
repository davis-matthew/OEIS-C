/*
 * A154300  Primes of the form (1+2+...+m)/57 = A000217(m)/57.
 * https://oeis.org/A154300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A154300: primes of the form T(m)/57, where T(m)=m(m+1)/2
   Non‑nonn, finite; only m = 18, 38, 57, 113 give primes.
   The function computes the list once and returns the n‑th term.
   Out of bounds indices return 0.
*/
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static state: the list of primes and its length */
    static seq_t primes[4];
    static size_t count = 0;
    static bool ready   = false;

    if (!ready)
    {
        const idx_t LIM = 2 * 57;          /* 114 */
        for (idx_t m = 1; m <= LIM; ++m)
        {
            /* compute T(m) in an intermediate unsigned 64‑bit type */
            uint64_t mm = (uint64_t)m;
            uint64_t t  = mm * (mm + 1) / 2;
            if (t % 57 != 0)
                continue;
            uint64_t p = t / 57;
            if (is_prime_big((seq_t)p))
                primes[count++] = (seq_t)p;
        }
        ready = true;
    }

    if (n == 0 || n > count)
        return 0;      /* no term for this index */
    return primes[n - 1];
}
