/*
 * A094802  a(n) = smallest k such that all of 1 through n divides k!.
 * https://oeis.org/A094802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max_{p ≤ n} k where v_p(k!) ≥ ⌊log_p(n)⌋
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   min_k_for_prime, vp_fac
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Auxiliary sieve to obtain primes up to n.
    All computations are performed with idx_t; the final result is cast to seq_t.
    The function a(idx_t) is externally visible and matches the interface
    required by the test harness. */

#include <stdlib.h>
#include <string.h>
#include "../utils/min_k_for_prime.h"
#include "../utils/vp_fac.h"

/* static data for prime list */
static int *s_primes = NULL;
static size_t s_prime_cnt = 0;
static idx_t s_sieve_lim = 0;

/* Ensure primes up to 'limit' are available.
   The routine uses a simple sieve; it is called only when needed. */
static void ensure_primes(idx_t limit)
{
    if (limit <= s_sieve_lim) return;

    free(s_primes);
    s_primes   = NULL;
    s_prime_cnt = 0;

    size_t n = (size_t)limit;
    /* odd size fits in memory for the problem limits */
    char *is_prime = (char*)malloc(n + 1);
    if (!is_prime) return;   /* allocation failed – caller will see a( ) = 0 */

    memset(is_prime, 1, n + 1);
    if (n >= 0) is_prime[0] = 0;
    if (n >= 1) is_prime[1] = 0;

    for (size_t i = 2; i * i <= n; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= n; j += i)
                is_prime[j] = 0;
        }
    }

    size_t cnt = 0;
    for (size_t i = 2; i <= n; ++i)
        if (is_prime[i]) ++cnt;

    s_primes = (int*)malloc(cnt * sizeof(int));
    if (!s_primes) {
        free(is_prime);
        return;
    }

    size_t idx = 0;
    for (size_t i = 2; i <= n; ++i)
        if (is_prime[i]) s_primes[idx++] = (int)i;

    s_prime_cnt = cnt;
    free(is_prime);
    s_sieve_lim = limit;
}

/* ------------------------------------------------------------ */
/*  Sequence term a(n)                                         */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n=0, return 0 */
    if (n == 1) return (seq_t)1;     /* a(1) = 1 */

    /* obtain primes up to n only once */
    ensure_primes(n);

    idx_t best = 0;

    for (size_t i = 0; i < s_prime_cnt; ++i) {
        idx_t p = (idx_t)s_primes[i];
        if (p > n) break;

        /* exponent of p in lcm(1..n) = floor(log_p(n)) */
        idx_t e   = 0;
        idx_t val = p;
        while (val <= n) {
            ++e;
            if (val > n / p) break;
            val *= p;
        }

        idx_t kp = min_k_for_prime(p, e);
        if (kp > best) best = kp;
    }

    return (seq_t)best;
}
