/*
 * A189892  a(n) = n*prime(n) - sum_{i=1..n-1} prime(i).
 * https://oeis.org/A189892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*prime(n) - sum_{i=1..n-1} prime(i)
 * Range:     u8 n <= 11, u16 n <= 143, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* External signature */
extern idx_t is_prime(idx_t x);   /* not used but kept for compatibility */
extern void free_seq(void *p);    /* placeholder for any cleanup */

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n==0, guard against UB */

    /* Static cache of primes and cumulative sums */
    static uint64_t *primes = NULL;      /* primes[i] = (i+0)-th prime (0-indexed) */
    static size_t prime_cap   = 0;        /* allocated size of primes array */
    static size_t prime_cnt   = 0;        /* actual number of primes stored */

    static seq_t *cum   = NULL;          /* cum[i] = sum of first i primes, cum[0]=0 */
    static size_t cum_cap = 0;            /* allocated size of cum array */

    /* Ensure at least n primes are known */
    if (prime_cnt < (size_t)n) {
        /* Initial allocation if empty */
        if (prime_cap == 0) {
            prime_cap = 16;
            primes    = (uint64_t*)malloc(prime_cap * sizeof(uint64_t));
            cum       = (seq_t*)malloc((prime_cap + 1) * sizeof(seq_t));
            if (!primes || !cum) return 0;   /* allocation failure */
            /* Add prime 2 as the first prime */
            primes[0] = 2;
            cum[0] = 0;
            cum[1] = (seq_t)2;
            prime_cnt = 1;
            cum_cap = prime_cap + 1;
        }

        /* Candidate for next prime */
        uint64_t candidate = (prime_cnt == 0) ? 2 : primes[prime_cnt - 1] + 1;
        if (candidate % 2 == 0) candidate++;   /* start from first odd after 2 */

        while (prime_cnt < (size_t)n) {
            int is_p = 1;
            uint64_t lim = (uint64_t)(candidate * candidate <= UINT64_MAX
                                        ? candidate : UINT64_MAX);
            for (size_t i = 0; i < prime_cnt; ++i) {
                uint64_t p = primes[i];
                if (p * p > candidate) break;
                if (candidate % p == 0) { is_p = 0; break; }
            }
            if (is_p) {
                /* Ensure space for new prime */
                if (prime_cnt == prime_cap) {
                    size_t new_cap = prime_cap * 2;
                    uint64_t *tmp = (uint64_t*)realloc(primes, new_cap * sizeof(uint64_t));
                    if (!tmp) return 0;  /* allocation failure */
                    primes = tmp;
                    prime_cap = new_cap;

                    seq_t *tmpcum = (seq_t*)realloc(cum, (new_cap + 1) * sizeof(seq_t));
                    if (!tmpcum) return 0;
                    cum = tmpcum;
                    cum_cap = new_cap + 1;
                }
                primes[prime_cnt] = candidate;
                /* Update cumulative sum */
                cum[prime_cnt + 1] = cum[prime_cnt] + (seq_t)candidate;
                ++prime_cnt;
            }
            candidate += 2;   /* test only odd numbers */
        }
    }

    /* Compute a(n) = n * prime(n) - sum_{i=1..n-1} prime(i) */
    seq_t sum_prev = cum[n - 1];               /* sum of first n-1 primes */
    seq_t prime_n = (seq_t)primes[n - 1];      /* nth prime (1-indexed) */
    return ((seq_t)n) * prime_n - sum_prev;
}
