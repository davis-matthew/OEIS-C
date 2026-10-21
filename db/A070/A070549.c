/*
 * A070549  a(n) = Cardinality{ k in range 1 <= k <= n such that Moebius(k) =
 *          -1 }.
 * https://oeis.org/A070549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Cardinality{ k : 1 ≤ k ≤ n and μ(k) = -1 }
 * Range:     u8 n <= 829, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A070549: count of numbers with Möbius value -1 up to n. */

/* We compute μ(k) by a linear sieve each call (n ≤ ~10⁴ is trivial) and
 * then count the -1 values. The algorithm is O(n) in time and O(n) in
 * memory per call, easily satisfying the required speed. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Allocate arrays for μ and the is‑composite flag. */
    size_t N = (size_t)n + 1;                     /* array size */
    int   *mu      = (int*)malloc(N * sizeof(int));   /* μ values : -1,0,1 */
    unsigned char *is_comp = (unsigned char*)malloc(N * sizeof(unsigned char)); /* composite flag */
    if (!mu || !is_comp) return 0;                /* allocation failure → 0 */

    /* Allocate primes array – at most n/2 primes for n>1. */
    size_t primes_cap = (n < 2) ? 1 : (size_t)n / 2 + 1;
    int *primes = (int*)malloc(primes_cap * sizeof(int));
    if (!primes) { free(mu); free(is_comp); return 0; }

    /* Initialize. */
    mu[0] = 0;     /* unused */
    mu[1] = 1;
    for (size_t i = 2; i < N; ++i) is_comp[i] = 0;
    size_t prime_cnt = 0;

    /* Linear sieve to compute μ up to n. */
    for (size_t i = 2; i <= (size_t)n; ++i) {
        if (!is_comp[i]) {                /* i is prime */
            primes[prime_cnt++] = (int)i;
            mu[i] = -1;
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            int p = primes[j];
            unsigned long long ip = (unsigned long long)i * (unsigned long long)p;
            if (ip > (unsigned long long)n) break;
            size_t ip_idx = (size_t)ip;
            is_comp[ip_idx] = 1;
            if (i % p == 0) {              /* p divides i – square factor */
                mu[ip_idx] = 0;
                break;
            } else {
                mu[ip_idx] = -mu[i];
            }
        }
    }

    /* Count μ(k) = -1 for 1 ≤ k ≤ n. */
    seq_t count = 0;
    for (size_t i = 1; i <= (size_t)n; ++i) {
        if (mu[i] == -1) ++count;
    }

    /* Clean up. */
    free(mu);
    free(is_comp);
    free(primes);

    return count;
}
