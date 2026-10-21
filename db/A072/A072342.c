/*
 * A072342  a(n) = the least natural number k such that k*reverse(n) + 1 is
 *          prime.
 * https://oeis.org/A072342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = least natural number k such that k*reverse(n)+1 is prime
 * Range:     u8 n <= 12, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   isPrimeGeneric, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/rev.h"
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* No valid term for n < 1; the sequence is defined for n ≥ 1 */
    if (n < 1) return 0;

    /* reverse(n) is returned as an idx_t; cast it to seq_t for arithmetic */
    seq_t rev_n = (seq_t)rev(n);

    for (idx_t k = 1; ; ++k) {
        seq_t candidate = rev_n * k + 1;          /* k*reverse(n)+1 */
        if (isPrimeGeneric(candidate))            /* primality test on seq_t */
            return (seq_t)k;                     /* a(n) is the first such k */
        /* The conjecture states a(n) ≤ n for all n, but the search is
           unbounded to avoid dependence on the conjecture. */
    }
}
