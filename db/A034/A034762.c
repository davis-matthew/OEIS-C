/*
 * A034762  Dirichlet convolution of primes (with 1) with sigma(n).
 * https://oeis.org/A034762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} f(d)·σ(n/d) where σ(m) is the sum of all divisors
 *            of m and f(1) = 1, f(k) = prime(k-1) for k ≥ 2
 * Range:     u8 n <= 17, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function pre‑computes σ and f up to the required index,
   re‑computing from scratch when a larger n is requested.  */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ---- global cache -------------------------------------------- */
static seq_t *sigma_arr = NULL;   /* σ[1..N] */
static seq_t *f_arr     = NULL;   /* f[1..N] */
static uint32_t *primes = NULL;   /* primes[0..P-1]  */
static size_t primes_cnt = 0;     /* number of primes stored   */
static idx_t  current_N = 0;      /* highest index that has been cached */

/* ---- helper to build primes up to an index ------------------- */
static void build_primes_upto_idx(size_t needed_cnt, size_t limit)
{
    if (primes_cnt < needed_cnt)
    {
        /* allocate a new array, discarding the old one   */
        free(primes);
        primes = (uint32_t *)malloc(needed_cnt * sizeof(uint32_t));
        if (!primes) abort();
        primes_cnt = needed_cnt;

        /* simple sieve of Eratosthenes up to 'limit'   */
        uint8_t *is_prime = (uint8_t *)calloc(limit + 1, sizeof(uint8_t));
        if (!is_prime) abort();
        for (size_t i = 2; i <= limit; ++i) is_prime[i] = 1;

        for (size_t i = 2; i * i <= limit; ++i)
            if (is_prime[i])
                for (size_t j = i * i; j <= limit; j += i)
                    is_prime[j] = 0;

        /* collect primes into our array */
        size_t cnt = 0;
        for (size_t i = 2; i <= limit && cnt < needed_cnt; ++i)
            if (is_prime[i]) primes[cnt++] = (uint32_t)i;

        free(is_prime);
        if (cnt < needed_cnt)
            abort();   /* should never happen for the chosen limit */
    }
}

/* ---- helper to recompute σ[1..N] and f[1..N] ----------------- */
static void recompute_up_to(idx_t n)
{
    if (n <= current_N) return;
    if (n == 0) return;

    size_t N = (size_t)n;

    /* ---- compute σ array ------------------------------------ */
    free(sigma_arr);
    sigma_arr = (seq_t *)calloc(N + 1, sizeof(seq_t));
    if (!sigma_arr) abort();
    for (size_t i = 1; i <= N; ++i)
        for (size_t j = i; j <= N; j += i)
            sigma_arr[j] += (seq_t)i;

    /* ---- compute f array ------------------------------------ */
    free(f_arr);
    f_arr = (seq_t *)calloc(N + 1, sizeof(seq_t));
    if (!f_arr) abort();

    f_arr[1] = (seq_t)1;          /* f(1) = 1 */

    /* we need primes[0..N-2] (for n ≥ 2) */
    size_t needed_primes = (N >= 2 ? N - 1 : 0);

    /* safe upper bound for the N‑th prime: use p_n < n(20) for n ≤ 10⁵ */
    size_t sieve_limit = N * 20 + 100;
    if (sieve_limit < 20) sieve_limit = 20;    /* safety for tiny N */

    build_primes_upto_idx(needed_primes, sieve_limit);

    for (size_t d = 2; d <= N; ++d)
        f_arr[d] = (seq_t)primes[d - 2];

    current_N = N;
}

/* ---- public sequence function --------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    size_t N = (size_t)n;

    recompute_up_to(n);

    seq_t result = 0;
    for (size_t d = 1; d * d <= N; ++d)
    {
        if (N % d != 0) continue;
        size_t m = N / d;

        seq_t fd = f_arr[d];
        seq_t sm = sigma_arr[m];
        result += fd * sm;

        if (d != m)
        {
            seq_t fm = f_arr[m];
            seq_t sd = sigma_arr[d];
            result += fm * sd;
        }
    }
    return result;
}
