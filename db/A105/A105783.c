/*
 * A105783  Number of terms among the first n primes that are divisors of the
 *          sum of the first n primes.
 * https://oeis.org/A105783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes among first n primes that divide the sum
 *            of the first n primes
 * Range:     u8 n <= 13, u16 n <= 1550, u32 n <= 9823, u64 n <= 9823,
 *            u128 n <= 9823, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n<1, return 0 */

    size_t nn = (size_t)n;                /* local copy for array sizes */
    uint64_t *primes = (uint64_t*)malloc(nn * sizeof(uint64_t));
    if (!primes) abort();                 /* out of memory */

    seq_t sum   = 0;
    size_t pcnt = 0;
    uint64_t cand = 2;

    /* generate the first n primes */
    while (pcnt < nn) {
        int is_prime = 1;
        for (size_t j = 0; j < pcnt; ++j) {
            uint64_t p = primes[j];
            if (p > cand / p) break;      /* p*p > cand */
            if (cand % p == 0) { is_prime = 0; break; }
        }
        if (is_prime) {
            primes[pcnt] = cand;
            sum += (seq_t)cand;           /* accumulate sum   */
            ++pcnt;
        }
        if (cand == 2) cand = 3;          /* after 2 skip even numbers */
        else cand += 2;
    }

    /* count how many of these primes divide the sum */
    seq_t result = 0;
    for (size_t i = 0; i < nn; ++i) {
        seq_t p = (seq_t)primes[i];
        if (sum % p == 0) ++result;
    }

    free(primes);
    return result;
}
