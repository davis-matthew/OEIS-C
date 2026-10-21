/*
 * A089730  Greatest prime factor of all differences prime(n)-q, q prime and
 *          q<prime(n); a(1)=0.
 * https://oeis.org/A089730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = greatest prime factor of all differences prime(n)-q,
 *            where q runs over all primes less than prime(n)
 * Range:     u8 n <= 56, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static size_t _prime_cnt = 0;          /* number of primes stored          */
static uint32_t *_prime_arr = NULL;    /* array of primes (0‑based)        */

static size_t _lpf_limit = 0;          /* size of lpf array + 1            */
static uint32_t *_lpf_arr = NULL;      /* lpf[t] = largest prime factor of t */

/* Ensure that we have at least the first 'need' primes and an lpf array up to
   a limit that covers prime(need).  We over‑estimate the limit by
   need*20+100, which is comfortably larger than the 10 000th prime 104 729. */
static void _ensure_sieve(idx_t need)
{
    idx_t limit = need * 20 + 100;           /* crude upper bound  */
    if (limit <= _lpf_limit) return;         /* already enough */

    /* free previous data current arrays                                  */
    free(_prime_arr);
    free(_lpf_arr);

    _prime_cnt = 0;
    _prime_arr = NULL;
    _lpf_limit = limit;

    /* sieve of Eratosthenes for primes and for largest prime factor   */
    unsigned char *is_prime = (unsigned char *)malloc((size_t)limit + 1);
    if (!is_prime) exit(EXIT_FAILURE);
    memset(is_prime, 1, (size_t)limit + 1);
    is_prime[0] = is_prime[1] = 0;

    _lpf_arr = (uint32_t *)malloc((size_t)(limit + 1) * sizeof(uint32_t));
    if (!_lpf_arr) exit(EXIT_FAILURE);
    memset(_lpf_arr, 0, (size_t)(limit + 1) * sizeof(uint32_t));

    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_prime[i]) continue;                 /* composite */

        /* store the prime                                               */
        if (_prime_cnt % 64 == 0) {                 /* grow by 64 each time */
            uint32_t *tmp = (uint32_t *)realloc(_prime_arr,
                                 (_prime_cnt + 64) * sizeof(uint32_t));
            if (!tmp) exit(EXIT_FAILURE);
            _prime_arr = tmp;
        }
        _prime_arr[_prime_cnt++] = (uint32_t)i;

        /* mark multiples and set their largest prime factor            */
        for (idx_t j = i; j <= limit; j += i) {
            _lpf_arr[j] = (uint32_t)i;              /* i is the current, largest prime so far */
            is_prime[j] = 0;                       /* mark composite (prime will be reset) */
        }
        is_prime[i] = 1;                            /* reset prime flag */
    }

    free(is_prime);
}

/* The main sequence function                                            */
seq_t a(idx_t n)
{
    if (n <= 1) return 0;                  /* a(1) = 0 */

    _ensure_sieve((size_t)n);              /* primes and lpf up to n‑th prime */

    seq_t prime_n = (seq_t)_prime_arr[(size_t)n - 1];
    seq_t max_lpf = 1;

    for (size_t i = 0; i + 1 < (size_t)n; ++i) {
        seq_t diff = prime_n - (seq_t)_prime_arr[i];
        seq_t lpf_val = (seq_t)_lpf_arr[(size_t)diff];  /* fits in 64 bits */
        if (lpf_val > max_lpf) max_lpf = lpf_val;
    }
    return max_lpf;
}
