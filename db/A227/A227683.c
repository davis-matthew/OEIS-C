/*
 * A227683  Number of digits in n-th Mersenne number.
 * https://oeis.org/A227683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(p_n * log10(2)) + 1, where p_n is the n-th prime
 *            (OEIS A001348)
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                       /* undefined for n < 1 */

    /* nth prime, must fit in uint32_t for n up to the limits of the utils. */
    uint32_t p = get_prime(n);

    /* 0.3010299956639811952 approximated as NUM/DEN  (precise to >1e-18).
       NUM/DEN = log10(2)  exactly for this rational. */
    const unsigned long long NUM = 3010299956639811952ULL;
    const unsigned long long DEN = 10000000000000000000ULL;   /* 1e19 */

    /* Compute floor(p * NUM / DEN) with 128-bit arithmetic to avoid overflow. */
    unsigned __int128 prod   = (unsigned __int128)p * NUM;
    unsigned long long floor = (unsigned long long)(prod / DEN);

    return (seq_t)floor + 1;
}
