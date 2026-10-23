/*
 * A196227  Number of 2 X 2 integer matrices with elements from {1,...,n}
 *          whose determinant is 1.
 * https://oeis.org/A196227
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -2*(n + 1) + 4*Σ_{k=1..n} φ(k) for n > 0, a(0)=0
 * Range:     u8 n <= 14, u16 n <= 232, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of 2×2 integer matrices with entries in {1,…,n}
   whose determinant is ±1.  Uses an incremental sieve of Euler's totient. */

#include <stdlib.h>   /* malloc, realloc */

/* ---------- Euler's totient for a single integer ---------- */
static seq_t phi_simple(idx_t m)
{
    if (m <= 1)
        return (seq_t)m;   /* φ(0)=0, φ(1)=1 */

    seq_t result = (seq_t)m;
    idx_t n = m;
    for (idx_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            while (n % p == 0)
                n /= p;
            result -= result / (seq_t)p;
        }
    }
    if (n > 1)
        result -= result / (seq_t)n;

    return result;
}

/* ---------- Cached cumulative sums of φ up to the maximum n seen ---------- */
static seq_t *phi_cum = NULL;  /* phi_cum[i] = Σ_{k=1..i} φ(k) */
static size_t   phi_cum_cap = 0;
static idx_t    phi_cum_max = 0;

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Grow the cache if necessary */
    if (n > phi_cum_max) {
        size_t new_cap = (size_t)(n + 1);
        if (phi_cum == NULL) {
            phi_cum = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (!phi_cum) abort();          /* out of memory */
            phi_cum[0] = 0;
            phi_cum_cap = new_cap;
        } else if (new_cap > phi_cum_cap) {
            seq_t *tmp = (seq_t *)realloc(phi_cum, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            phi_cum = tmp;
            phi_cum_cap = new_cap;
        }

        /* Compute φ for new indices and accumulate sums */
        for (idx_t k = phi_cum_max + 1; k <= n; ++k) {
            seq_t pk = phi_simple(k);
            phi_cum[k] = phi_cum[k - 1] + pk;
        }
        phi_cum_max = n;
    }

    seq_t sum_phi = phi_cum[n];
    seq_t result = 4 * sum_phi - 2 * ((seq_t)n + 1);
    return result;
}
