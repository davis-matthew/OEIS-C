/*
 * A082650  Number of primes < n of the form 1 + k*lpf(n), where lpf(n) is the
 *          least prime factor of n (A020639).
 * https://oeis.org/A082650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1621, u16 n <= 9425, u32 n <= 9425, u64 n <= 9425,
 *            u128 n <= 9425, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = number of primes q < n with q ≡ 1 (mod lpf(n)) */
#include <stdlib.h>

/* Global sieve data shared by all calls */
static idx_t *spf       = NULL;   /* least prime factor of each integer */
static idx_t *prime_list= NULL;   /* array of all primes in increasing order */
static size_t prime_cnt = 0;      /* number of primes stored */
static size_t prime_cap = 0;      /* capacity of prime_list */
static idx_t  max_pre    = 0;     /* largest n for which the sieve is valid */

/* Grow the sieve so that it contains data up to at least n */
static void ensure_sieve(idx_t n)
{
    if (n <= max_pre) return;          /* already covers the range */

    /* Allocate new tables */
    idx_t *new_spf   = (idx_t*)calloc((size_t)(n + 1), sizeof(idx_t));
    if (!new_spf) return;              /* allocation failed – caller will return 0 */

    if (prime_cap == 0) prime_cap = 1024;
    idx_t *new_primes = (idx_t*)malloc(prime_cap * sizeof(idx_t));
    if (!new_primes) {
        free(new_spf);
        return;
    }

    /* Linear (Euler) sieve */
    size_t cnt = 0;
    for (idx_t i = 2; i <= n; ++i) {
        if (new_spf[i] == 0) {
            new_spf[i] = i;
            if (cnt == prime_cap) {
                prime_cap <<= 1;
                void *tmp = realloc(new_primes, prime_cap * sizeof(idx_t));
                if (!tmp) {               /* allocation failed – clean up */
                    free(new_spf);
                    free(new_primes);
                    return;
                }
                new_primes = (idx_t*)tmp;
            }
            new_primes[cnt++] = i;
        }
        for (size_t j = 0; j < cnt &&
                         new_primes[j] <= new_spf[i] &&
                         i * new_primes[j] <= n; ++j) {
            idx_t v = i * new_primes[j];
            if (new_spf[v] == 0) new_spf[v] = new_primes[j];
        }
    }

    /* Replace old tables with the new ones */
    free(spf);
    free(prime_list);
    spf          = new_spf;
    prime_list   = new_primes;
    prime_cnt    = cnt;
    max_pre      = n;
}

/* Function from the OEIS entry A082650 */
seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* a(1) = 0, a(0) undefined but return 0 */

    ensure_sieve(n);

    /* If n itself is prime, lpf(n)=n and there are no primes < n of form 1+kp with p=n */
    if (spf[n] == n) return 0;

    idx_t p = spf[n];               /* least prime factor of n */
    seq_t cnt = 0;

    /* Count primes q < n that satisfy (q-1) % p == 0 */
    for (size_t i = 0; i < prime_cnt && prime_list[i] < n; ++i) {
        if ((prime_list[i] - 1) % p == 0) {
            ++cnt;
        }
    }
    return cnt;
}
