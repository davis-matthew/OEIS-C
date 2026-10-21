/*
 * A060230  Smaller of twin primes whose middle term is a multiple of
 *          A002110(4)=210.
 * https://oeis.org/A060230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th smaller prime of a twin prime pair * p, p+2,
 *            where p ≡ 209 (mod 210). The sequence * begins 419, 1049, 2309,
 *            … (p = 210*k – 1)
 * Range:     u8 none, u16 n <= 54, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"   /* provides bool is_prime_u64(uint64_t) */

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 1 (offset = 1). */
    if (n == 0)
        return (seq_t)0;   /* behaviour for n < 1 is irrelevant */

    idx_t found = 0;          /* how many valid primes we have seen */
    uint64_t k = 1;           /* multiplier for the pattern p = 210*k – 1 */

    while (1) {
        uint64_t p = 210ULL * k - 1;  /* smaller member of the twin pair */
        ++k;

        /* Both p and p+2 must be prime. */
        if (is_prime_u64(p) && is_prime_u64(p + 2)) {
            ++found;
            if (found == n)
                return (seq_t)p;   /* cast fits for all supported seq_t types */
        }
    }
}
