/*
 * A065309  a(n) = prime(n) - prime(n - pi(n)).
 * https://oeis.org/A065309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) - prime(n - π(n))
 * Range:     u8 n <= 156, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A065309.  Uses a dynamic sieve to
 * generate the first N primes and the prime‑count function π(n).
 * The algorithm is O(limit log log limit) for the largest n needed.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static /* integer type for indices */
    uint64_t max_n_done = 0;            /* last n for which primes were prepared */
static uint32_t *prime_list = NULL;     /* array of primes (1‑based) */
static size_t  prime_cnt = 0;           /* number of primes stored */
static uint32_t *pi_array = NULL;       /* π(i) for i up to sieve_limit (0‑based) */
static size_t  sieve_limit = 0;         /* current sieve size */

/* ------------------------------------------------------------------
 * Sieve of Eratosthenes up to 'limit'.  If limit is smaller than the
 * current one, nothing is done.  Otherwise a new sieve is built, primes
 * are recorded, and π() array is updated.
 * ------------------------------------------------------------------ */
static void prepare_primes_upto(uint64_t n)
{
    if (n <= max_n_done) return;               /* already prepared */

    /* Recommended upper bound: 20*n + 10  (enough for n up to millions) */
    uint64_t limit = (n < 6) ? 15 : 20 * n + 10;

    /* Ensure we have enough primes for index 'n' */
    for (;;) {
        /* Simple sieve using a byte array */
        uint8_t *sieve = (uint8_t *)calloc(limit + 1, 1);
        if (!sieve) exit(1);
        sieve[0] = sieve[1] = 1;                     /* not prime */
        uint64_t sq = (uint64_t)sqrt((double)limit);
        for (uint64_t i = 2; i <= sq; ++i)
            if (!sieve[i]) {
                for (uint64_t j = i * i; j <= limit; j += i)
                    sieve[j] = 1;
            }

        /* Count primes and build π() array */
        uint32_t *pi = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
        if (!pi) exit(1);
        uint32_t cnt = 0;
        for (uint32_t i = 0; i <= limit; ++i) {
            if (!sieve[i]) ++cnt;
            pi[i] = cnt;
        }
        if (cnt >= n) {
            /* Store primes list */
            uint32_t *plist = (uint32_t *)malloc(cnt * sizeof(uint32_t));
            if (!plist) exit(1);
            uint32_t idx = 0;
            for (uint32_t i = 2; i <= limit; ++i)
                if (!sieve[i])
                    plist[idx++] = i;

            /* Replace old data */
            free(prime_list);
            free(pi_array);
            free(sieve);

            prime_list = plist;
            prime_cnt = cnt;
            pi_array = pi;
            sieve_limit = limit;
            max_n_done = n;
            return;
        }

        /* Not enough primes; double limit and try again */
        free(pi);
        free(sieve);
        limit *= 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety, though n>=1 per problem */

    prepare_primes_upto(n);                 /* ensure prime data for index n */

    /* prime indices are 1‑based: prime_list[0] = 2 = prime(1) */
    seq_t prime_n = (seq_t)prime_list[n - 1];
    seq_t pi_n   = (seq_t)pi_array[n];
    idx_t second_index = n - (idx_t)pi_n;   /* n - π(n)  (1‑based) */
    seq_t prime_second = (seq_t)prime_list[second_index - 1];

    return prime_n - prime_second;
}
