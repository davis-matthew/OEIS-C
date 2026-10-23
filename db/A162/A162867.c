/*
 * A162867  a(n) is the sum of all possible pairs of the first n primes.
 * https://oeis.org/A162867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)*sum_{k=1}^{n} p_k (p_k = k-th prime)
 * Range:     u8 n <= 5, u16 n <= 32, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

/* ------------------------------------------------------------------ */
/* Estimate upper bound for the n-th prime (valid for n >= 6)          */
/* ------------------------------------------------------------------ */
static idx_t prime_limit_for_n(idx_t n)
{
    if (n < 6) return 15;                           /* small fixed bound   */
    double nn  = (double)n;
    double lim = nn * (log(nn) + log(log(nn))) + 3.0;
    return (idx_t)lim;
}

/* ------------------------------------------------------------------ */
/* Global static data                                                 */
/* ------------------------------------------------------------------ */
static uint64_t  *prime_buf = NULL;  /* array of primes (0-indexed)          */
static idx_t      prime_count = 0;   /* number of primes in prime_buf        */

static seq_t  *cum_buf = NULL;       /* cumul_sum[i] = sum of first i primes */
static idx_t   cum_count = 0;         /* size of cum_buf (>= prime_count+1)   */

/* ------------------------------------------------------------------ */
/* Recompute primes up to 'lim' and rebuild cumulative sums           */
/* ------------------------------------------------------------------ */
static void recompute_primes_and_cum(idx_t lim)
{
    /* ---------- sieve -------------------------------------------- */
    unsigned char *is_prime = (unsigned char*)malloc((size_t)(lim + 1));
    if (!is_prime) exit(EXIT_FAILURE);

    for (idx_t i = 0; i <= lim; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (idx_t i = 2; i * i <= lim; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= lim; j += i)
                is_prime[j] = 0;
        }
    }

    /* ---------- count primes ------------------------------------ */
    idx_t cnt = 0;
    for (idx_t i = 2; i <= lim; ++i)
        if (is_prime[i]) ++cnt;

    /* ---------- store primes ------------------------------------- */
    uint64_t *buf = NULL;
    if (prime_buf) free(prime_buf);
    buf = (uint64_t*)malloc((size_t)cnt * sizeof(uint64_t));
    if (!buf) exit(EXIT_FAILURE);

    idx_t k = 0;
    for (idx_t i = 2; i <= lim; ++i) {
        if (is_prime[i]) {
            buf[k++] = (uint64_t)i;
        }
    }

    prime_buf  = buf;
    prime_count = cnt;

    free(is_prime);

    /* ---------- cumulative sums --------------------------------- */
    seq_t *cum = NULL;
    if (cum_buf) free(cum_buf);
    cum = (seq_t*)malloc((size_t)(cnt + 1) * sizeof(seq_t));
    if (!cum) exit(EXIT_FAILURE);

    cum[0] = (seq_t)0;
    for (idx_t i = 1; i <= cnt; ++i) {
        cum[i] = cum[i-1] + (seq_t)prime_buf[i-1];
    }

    cum_buf   = cum;
    cum_count = cnt;
}

/* ------------------------------------------------------------------ */
/* Ensure we have at least n primes and cum[0..n] ready               */
/* ------------------------------------------------------------------ */
static void ensure_primes_upto_count(idx_t n)
{
    if (prime_count >= n) return;          /* already enough */

    idx_t lim = prime_limit_for_n(n);
    recompute_primes_and_cum(lim);
    /* after recompute prime_count >= n by the bound used */
}

/* ------------------------------------------------------------------ */
/* Public function required by the framework                          */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined below offset, but safe */
    ensure_primes_upto_count(n);
    seq_t sum   = cum_buf[n];                /* sum of first n primes            */
    seq_t result = sum * (((seq_t)n) + 1);    /* a(n) = (n+1)*sum                 */
    return result;
}
