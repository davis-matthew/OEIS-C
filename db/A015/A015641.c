/*
 * A015641  Number of 5-tuples of different integers from [ 2,n ] with no
 *          global factor.
 * https://oeis.org/A015641
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d=1..n} μ(d)·C(floor(A(d)),5), where * A(d) is the
 *            number of integers in [2,n] divisible by d. * In this case
 *            A(1)=n-1 and A(d)=floor(n/d) for d>1. Thus *
 *            a(n)=C(n-1,5)+Σ_{d=2..n} μ(d)·C(floor(n/d),5)
 * Range:     u8 n <= 11, u16 n <= 27, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   choose5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/choose5.h"

/* static buffers for Möbius values, smallest prime factor and primes list */
static int   *mu_buf   = NULL; /* Möbius values */
static uint64_t *spf_buf   = NULL; /* smallest prime factor */
static uint64_t *prime_buf = NULL; /* list of primes */
static size_t   prime_sz   = 0;     /* number of primes stored */
static size_t   buf_cap    = 0;     /* current capacity of buffers */

/* ensure buffers are large enough for indices up to N (inclusive) */
static void ensure_buffers(size_t N)
{
    if (buf_cap >= N + 1) return;           /* already large enough */
    size_t new_cap = N + 1;
    int   *new_mu   = (int*)realloc(mu_buf,   new_cap * sizeof(int));
    uint64_t *new_spf  = (uint64_t*)realloc(spf_buf, new_cap * sizeof(uint64_t));
    uint64_t *new_prime  = (uint64_t*)realloc(prime_buf, new_cap * sizeof(uint64_t));
    if (!new_mu || !new_spf || !new_prime) abort(); /* allocation failed */
    mu_buf = new_mu;
    spf_buf = new_spf;
    prime_buf = new_prime;
    buf_cap = new_cap;
}

/* compute Möbius μ(d) for 1 ≤ d ≤ N using a linear sieve */
static void compute_mobius_up_to(size_t N)
{
    ensure_buffers(N);
    mu_buf[1] = 1;
    prime_sz = 0;
    for (size_t i = 2; i <= N; ++i) {
        spf_buf[i] = 0;
        mu_buf[i] = 0;
    }
    for (size_t i = 2; i <= N; ++i) {
        if (spf_buf[i] == 0) {                    /* i is prime */
            spf_buf[i] = i;
            prime_buf[prime_sz++] = i;
            mu_buf[i] = -1;
        }
        for (size_t j = 0; j < prime_sz; ++j) {
            uint64_t p = prime_buf[j];
            uint64_t v = i * p;
            if (v > N) break;
            spf_buf[v] = p;
            if (i % p == 0) {
                mu_buf[v] = 0;
                break;
            } else {
                mu_buf[v] = -mu_buf[i];
            }
        }
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 2) return 0;                 /* offset is 2, values below are undefined */
    /* compute Möbius values up to n */
    compute_mobius_up_to((size_t)n);

    seq_t total = choose5(n - 1);        /* the term for d = 1 */
    for (idx_t d = 2; d <= n; ++d) {
        int mu = mu_buf[d];
        if (mu == 0) continue;
        seq_t c = choose5(n / d);
        if (c == 0) continue;
        if (mu > 0) {
            total += c;
        } else {
            total -= c;
        }
    }
    return total;
}
