/*
 * A053785  Nextprime(n^4) - n^4.
 * https://oeis.org/A053785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = nextPrime(n^4) - n^4 Return the difference between the
 *            smallest prime strictly larger than n^4 and n^4 itself
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include primality test routine */
#include "../utils/isPrime.h"

seq_t a(idx_t n) {
    /* n is 1‑based, must be ≥1 */
    if (n == 0) return 0; /* undefined for n=0, return 0 */

    seq_t nsq = (seq_t)n * (seq_t)n;   /* n^2 */
    seq_t n4  = nsq * nsq;             /* n^4 (fits in uint64 for n≤1e4) */

    seq_t cand = n4 + 1;

    /* Handle the case n^4 = 1 -> cand == 2 */
    if (cand == 2) {
        return (seq_t)1; /* 2 - 1 = 1 */
    }

    /* Skip even numbers */
    if ((cand & 1) == 0) {
        cand += 1;
    }

    while (1) {
        /* isPrime accepts a 64‑bit unsigned integer (idx_t) */
        if (isPrime((idx_t)cand)) {
            return cand - n4;
        }
        cand += 2; /* next odd candidate */
    }
}
