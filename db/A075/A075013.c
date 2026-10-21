/*
 * A075013  Smallest k such that the decimal concatenation of k and k+1 is
 *          divisible by n.
 * https://oeis.org/A075013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest k such that the decimal concatenation of k and
 *            k+1 is divisible by n (OEIS A075013)
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* offset is 1, but guard */

    seq_t mod = (seq_t)n;                      /* work modulo n */
    seq_t pow10 = 10 % mod;                    /* 10¹  mod n  */
    seq_t threshold = 10;                      /* next power of 10  */
    seq_t k = 1;                               /* candidate k      */

    while (1) {
        /* If k+1 reaches a new power of 10, update pow10 and threshold */
        if (k + (seq_t)1 == threshold) {
            threshold *= 10;
            /* (pow10 * 10) % mod; use __int128 to avoid overflow on 64‑bit */
            pow10 = ( (unsigned __int128)pow10 * 10 ) % mod;
        }

        seq_t kmod = k % mod;
        seq_t part1 = ( (unsigned __int128)kmod * pow10 ) % mod;
        seq_t part2 = (k + (seq_t)1) % mod;
        seq_t remainder = (part1 + part2) % mod;

        if (remainder == 0)
            return k;                           /* found minimal k   */

        k += 1;
    }
}
