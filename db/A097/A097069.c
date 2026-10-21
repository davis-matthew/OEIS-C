/*
 * A097069  Positive integers n such that 2n - 9 is prime.
 * https://oeis.org/A097069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (p_{n+1} + 9) / 2, where p_k is the k-th prime with p_1 =
 *            2. (Thus a(1) = (p_2 + 9)/2 = (3+9)/2 = 6.)
 * Range:     u8 n <= 52, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* n >= 1 is guaranteed by the problem statement.  */
    /* Static storage for primes >= 3, indexing from 0. */
    static uint64_t *primes = NULL;     /* array of primes (>=3) */
    static size_t   count   = 0;        /* number of primes stored */
    static size_t   capacity= 0;        /* allocated capacity */

    if (primes == NULL) {
        /* initial allocation – enough for prime(n) up to ~10⁴ (≈105 000). */
        capacity = 20000;
        primes   = (uint64_t*)malloc(capacity * sizeof(uint64_t));
        count    = 0;
    }

    /* generate new primes until we have at least n of them (indexing from 1) */
    if (count < n) {
        uint64_t cand = (count == 0 ? 3 : primes[count-1] + 2);  /* next odd candidate */
        while (count < n) {
            int isprime = 1;
            for (size_t i = 0; i < count && primes[i]*primes[i] <= cand; ++i) {
                if (cand % primes[i] == 0) { isprime = 0; break; }
            }
            if (isprime) {
                if (count >= capacity) {                 /* grow storage */
                    capacity *= 2;
                    primes = (uint64_t*)realloc(primes, capacity * sizeof(uint64_t));
                }
                primes[count++] = cand;
            }
            cand += 2;    /* skip even numbers */
        }
    }

    /* compute (prime + 9)/2 using seq_t arithmetic */
    seq_t val = (seq_t)primes[n-1] + (seq_t)9;
    val /= 2;
    return val;
}
