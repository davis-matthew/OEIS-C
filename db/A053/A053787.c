/*
 * A053787  Nextprime(n^5) - n^5.
 * https://oeis.org/A053787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = next prime greater than n^5 minus n^5 The function
 *            computes n^5 as a seq_t (which may be big‑int on
 *            arbitrary‑precision builds), then searches upwards until it
 *            finds a prime with isPrimeGeneric(). The difference is returned
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* Compute n^5 */
    seq_t base = (seq_t)n;
    seq_t sq   = base * base;          /* n^2   */
    seq_t cub  = sq * base;            /* n^3   */
    seq_t p4   = cub * base;           /* n^4   */
    seq_t p5   = p4 * base;            /* n^5   */

    /* Find the next prime > n^5 */
    seq_t cand = p5 + 1;
    while (!isPrimeGeneric(cand))
        cand = cand + 1;

    return cand - p5;   /* difference: next prime minus n^5 */
}
