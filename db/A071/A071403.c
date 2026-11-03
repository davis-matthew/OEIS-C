/*
 * A071403  Which squarefree number is prime? a(n)-th squarefree number equals
 *          n-th prime.
 * https://oeis.org/A071403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=1}^{⌊√p_n⌋} μ(k) ⌊p_n / k²⌋, where p_n is the n‑th
 *            prime. This counts the square‑free numbers ≤ p_n
 * Range:     u8 n <= 80, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 8529 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* Pre‑computed Möbius function for 1 ≤ k ≤ 400 (more than enough for
   prime(10000) = 104 729). */
static int8_t mu_arr[400];
static bool mu_computed = false;

/* Compute μ(k) for 1 ≤ k ≤ 399 using a small sieve. */
static void init_mu(void)
{
    if (mu_computed) return;
    const int LIM = 399;
    int spf[LIM + 1];
    for (int i = 0; i <= LIM; ++i) spf[i] = i;
    for (int i = 2; i * i <= LIM; ++i) {
        if (spf[i] == i) {
            for (int j = i * i; j <= LIM; j += i)
                if (spf[j] == j) spf[j] = i;
        }
    }
    mu_arr[1] = 1;
    for (int i = 2; i <= LIM; ++i) {
        int p = spf[i];
        int m = i / p;
        if (m % p == 0) mu_arr[i] = 0;
        else mu_arr[i] = -mu_arr[m];
    }
    mu_computed = true;
}

/* Count square‑free numbers ≤ N using inclusion–exclusion. */
static seq_t count_squarefree_le(uint32_t N)
{
    init_mu();
    int maxk = 0;
    /* compute floor(sqrt(N)) – N is ≤ 104 729 so a small loop is fine */
    while ((uint64_t)(maxk + 1) * (maxk + 1) <= N) ++maxk;

    int64_t acc = 0;
    for (int k = 1; k <= maxk; ++k) {
        int8_t mu = mu_arr[k];
        if (mu == 0) continue;
        uint32_t d2 = (uint32_t)k * (uint32_t)k;
        uint32_t term = N / d2;
        if (mu > 0) acc += term;
        else        acc -= term;
    }
    return (seq_t)acc;
}

seq_t a(idx_t n)
{
    /* n is 1‑based and guaranteed ≥ 1. */
    uint32_t p = get_prime(n);          /* n‑th prime */
    return count_squarefree_le(p);
}
