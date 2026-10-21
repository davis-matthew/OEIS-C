/*
 * A085434  Twice odd isolated primes.
 * https://oeis.org/A085434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2 * (n-th odd isolated prime)
 * Range:     u8 n <= 11, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* An odd isolated prime is an odd prime p such that p-2 and p+2 are not primes. */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of the isolated primes found so far */
    static seq_t *isolated      = NULL;   /* array of isolated odd primes  */
    static size_t isolated_len  = 0;      /* number of isolated primes stored */
    static size_t isolated_cap  = 0;      /* capacity of isolated array       */
    static idx_t sieve_limit    = 0;      /* current sieve upper bound       */

    /* initialise cache once */
    if (isolated == NULL) {
        isolated_cap = 1024;
        isolated = (seq_t*)malloc(isolated_cap * sizeof(seq_t));
        if (isolated == NULL) exit(1);
    }

    /* enlarge the sieve and recompute the isolated primes until we have n of them */
    while (isolated_len < (size_t)n) {
        /* grow the sieve range */
        idx_t new_limit = (sieve_limit == 0) ? 100 : sieve_limit * 2U;

        /* allocate boolean array for the sieve */
        uint8_t *is_prime = (uint8_t*)malloc((size_t)new_limit + 1);
        if (is_prime == NULL) exit(1);

        /* initialise */
        for (idx_t i = 0; i <= new_limit; ++i) is_prime[i] = 1;
        is_prime[0] = is_prime[1] = 0;

        /* classic Eratosthenes sieve */
        for (idx_t i = 2; i * i <= new_limit; ++i) {
            if (is_prime[i]) {
                for (idx_t j = i * i; j <= new_limit; j += i)
                    is_prime[j] = 0;
            }
        }

        /* ensure capacity for isolated array */
        size_t new_cap = new_limit / 10 + 64;
        if (new_cap > isolated_cap) {
            seq_t *tmp = (seq_t*)realloc(isolated, new_cap * sizeof(seq_t));
            if (tmp == NULL) exit(1);
            isolated = tmp;
            isolated_cap = new_cap;
        }

        /* rebuild the list of isolated odd primes from the new sieve */
        size_t cnt = 0;
        for (idx_t p = 3; p + 2 <= new_limit; p += 2) {
            if (is_prime[p] && !is_prime[p-2] && !is_prime[p+2]) {
                if (cnt == isolated_cap) { /* double if needed */
                    size_t new_cap2 = isolated_cap * 2;
                    seq_t *tmp2 = (seq_t*)realloc(isolated, new_cap2 * sizeof(seq_t));
                    if (tmp2 == NULL) exit(1);
                    isolated = tmp2;
                    isolated_cap = new_cap2;
                }
                isolated[cnt++] = (seq_t)p;
            }
        }

        /* update state for next iteration if required */
        isolated_len = cnt;
        sieve_limit   = new_limit;

        free(is_prime);
    }

    /* return the required term: twice the (n‑th) isolated odd prime */
    seq_t result = isolated[(size_t)n - 1] * ((seq_t)2);
    return result;
}
