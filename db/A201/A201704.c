/*
 * A201704  Primes of the form 8*k^2 + 7.
 * https://oeis.org/A201704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 989, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 48 terms through n = 4785 (2026-11-03)
 * Helpers:   is_prime_u64, mulmod, powmod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes of the form 8*k^2 + 7 */
/* The function iterates k = 0, 1, 2, …, testing 8*k^2 + 7
   for primality with a deterministic Miller–Rabin routine
   valid for 64‑bit integers.  The nth found prime is returned. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mulmod.h"
#include "../utils/powmod_u64.h"

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t result = 0;

    for (uint64_t k = 0; ; ++k)
    {
        __int128 t = (__int128)8 * k * k + 7;
        uint64_t cand = (uint64_t)t;   /* safe for n up to about 10 000 */

        if (is_prime_u64(cand))
        {
            ++count;
            if (count == n)
            {
                result = (seq_t)cand;
                break;
            }
        }
    }
    return result;
}
