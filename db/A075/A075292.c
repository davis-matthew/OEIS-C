/*
 * A075292  Interprimes which are of the form s*prime, s=17.
 * https://oeis.org/A075292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 24, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A075292: Interprimes of the form 17 × prime.
    The sequence consists of the integers  a = 17·p (p prime) for which
    a is the average of two consecutive primes.  For k ≥ 1,
    a(k) = (p_i + p_{i+1})/2 where (p_i + p_{i+1})/2 is divisible by 17
    and the quotient is also prime.                                         */

#include <stdlib.h>
#include <string.h>

#define MAX_TERMS  25000          /* enough for n ≤ 10000 */
#define SIEVE_LIMIT  60000000U    /* primes up to this give >MAX_TERMS   */

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;          /* array of sequence terms */
    static size_t  cache_cnt = 0;        /* number of terms stored     */
    static unsigned char *is_prime = NULL; /* sieve bit‑array            */

    if (n == 0) return (seq_t)0;          /* offset is 1, guard          */

    /* initialise once */
    if (!cache) {
        /* allocate cache array */
#if defined(__cplusplus)
        cache = reinterpret_cast<seq_t *>(malloc(MAX_TERMS * sizeof(seq_t)));
        is_prime = reinterpret_cast<unsigned char *>(
                        malloc(SIEVE_LIMIT + 1));
#else
        cache = malloc(MAX_TERMS * sizeof(seq_t));
        is_prime = malloc(SIEVE_LIMIT + 1);
#endif
        if (!cache || !is_prime) return (seq_t)0;  /* allocation error */

        memset(is_prime, 1, SIEVE_LIMIT + 1);
        is_prime[0] = is_prime[1] = 0;

        /* sieve of Eratosthenes */
        for (size_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
            if (is_prime[i]) {
                for (size_t j = i * i; j <= SIEVE_LIMIT; j += i)
                    is_prime[j] = 0;
            }
        }

        /* collect interprimes of the required form */
        size_t prev_prime = 0;
        for (size_t num = 2; num <= SIEVE_LIMIT && cache_cnt < MAX_TERMS; ++num) {
            if (!is_prime[num]) continue;          /* not prime   */
            if (prev_prime) {
                size_t sum = prev_prime + num;     /* sum of neighbours */
                if ((sum & 1) == 0) {             /* average integer    */
                    size_t avg = sum >> 1;         /* (prev+next)/2      */
                    if ((avg % 17ULL) == 0) {
                        size_t q = avg / 17ULL;    /* quotient           */
                        if (q <= SIEVE_LIMIT && is_prime[q]) {
                            cache[cache_cnt++] = (seq_t)avg;
                        }
                    }
                }
            }
            prev_prime = num;
        }
        /* cache_cnt now holds the number of valid terms found   */
    }

    if (n > cache_cnt) return (seq_t)0;        /* out of range */
    return cache[n - 1];
}
