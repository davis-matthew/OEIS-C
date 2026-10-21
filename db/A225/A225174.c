/*
 * A225174  Square array read by antidiagonals: T(m,n) = greatest common
 *          unitary divisor of m and n.
 * https://oeis.org/A225174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, look
 * Formula:   a(n) = greatest common unitary divisor of the pair (i,j) * where
 *            (i,j) is the n‑th element visited in the antidiagonal traversal
 *            * of the infinite square array T(i,j) = unitary_gcd(i,j)
 * Range:     u8 n <= 9929, u16 n <= 9929, u32 n <= 9929, u64 n <= 9929,
 *            u128 n <= 9929, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* factorization: prime factors of n (num <= 2^64-1) */
static void factor(uint64_t n, uint64_t primes[32], unsigned exps[32], size_t *cnt)
{
    *cnt = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            unsigned e = 0;
            do { n /= p; ++e; } while (n % p == 0);
            primes[(*cnt)] = p;
            exps[(*cnt)]  = e;
            (*cnt)++;
        }
    }
    if (n > 1) {
        primes[(*cnt)] = n;
        exps[(*cnt)]   = 1;
        (*cnt)++;
    }
}

/* greatest common unitary divisor of a and b (unitary means the same
 * prime exponents appear in both). */
static seq_t unitary_gcd(seq_t a, seq_t b)
{
    uint64_t aa = (uint64_t)a, bb = (uint64_t)b;
    uint64_t pa[32], pb[32];
    unsigned ea[32], eb[32];
    size_t ca, cb;

    factor(aa, pa, ea, &ca);
    factor(bb, pb, eb, &cb);

    seq_t res = 1;
    for (size_t i = 0; i < ca; ++i) {
        for (size_t j = 0; j < cb; ++j) {
            if (pa[i] == pb[j]) {
                if (ea[i] == eb[j]) {
                    for (unsigned k = 0; k < ea[i]; ++k)
                        res *= (seq_t)pa[i];
                }
                break;
            }
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    /* find the diagonal d such that d*d+1/2 ≥ n */
    idx_t d = 1;
    while ((d * (d + 1)) / 2 < n) ++d;

    idx_t t_prev = ((d - 1) * d) / 2;   /* total terms before diagonal d */
    idx_t offset = n - t_prev - 1;      /* 0‑based index inside the diagonal */

    idx_t i = offset + 1;               /* first coordinate */
    idx_t j = d - offset;               /* second coordinate */

    return unitary_gcd((seq_t)i, (seq_t)j);
}
