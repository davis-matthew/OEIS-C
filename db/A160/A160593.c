/*
 * A160593  Indices of primes congruent to 11 modulo 12.
 * https://oeis.org/A160593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the index (1‑based) of the n‑th prime that is ≡ 11 (mod
 *            12)
 * Range:     u8 n <= 66, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>          /* for size_t, not used directly but required by some utils */
#include "../utils/is_prime.h"

/* The function is recursive in the sense that it scans primes sequentially
 * until it finds the n‑th prime congruent to 11 modulo 12, keeping track
 * of the overall prime index.  The scan uses the provided is_prime
 * routine and performs only integer arithmetic. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* offset is 1, guard anyway */

    idx_t found = 0;           /* how many qualifying primes have been seen */
    seq_t prime_index = 0;     /* 1‑based index in the list of all primes */

    for (idx_t p = 2; ; ++p)
    {
        if (is_prime(p))
        {
            ++prime_index;                    /* count this prime */
            if ((p % 12ULL) == 11ULL)         /* does it satisfy the congruence? */
            {
                ++found;
                if (found == n)
                    return prime_index;       /* found the n‑th one */
            }
        }
    }
}
