/*
 * A091088  a(n) is the minimum odd number that must be appended to n to form
 *          a prime.
 * https://oeis.org/A091088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn, less
 * Formula:   a(n) = minimal odd integer k such that the decimal concatenation
 *            of n and k is a prime. The implementation searches k = 1,3,5,…
 *            and tests each concatenation with the provided
 *            arbitrary‑precision primality routine
 * Range:     u8 n <= 19, u16 n <= 1426, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isPrimeGeneric.h"
#include "../utils/digit_len.h"

/* Return the smallest odd number that must be appended to n to form a prime. */
seq_t a(idx_t n)
{
    const seq_t base = n;                     /* n cast to seq_t once. */
    for (seq_t k = 1; ; k += 2)               /* iterate over odd candidates. */
    {
        /* Count decimal digits of k. */
        int d = digit_len(k);

        /* Compute 10^d in seq_t. */
        seq_t pow10 = 1;
        for (int i = 0; i < d; ++i)
            pow10 = pow10 * 10;

        /* Form the concatenated number n·10^d + k. */
        seq_t cand = base * pow10 + k;

        /* Stop at the first prime. */
        if (isPrimeGeneric(cand))
            return k;
    }
}
