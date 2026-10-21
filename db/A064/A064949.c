/*
 * A064949  a(n) = Sum_{i|n, j|n} min(i,j).
 * https://oeis.org/A064949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i|n, j|n} min(i,j)
 * Range:     u8 n <= 29, u16 n <= 2159, u32 n <= 9015, u64 n <= 9015,
 *            u128 n <= 9015, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* compare two uint64_t values for qsort */
static int cmp_uint64(const void *a, const void *b) {
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

/* factorise n, storing primes and exponents in the provided arrays.
   The arrays are sized at least 64, which is sufficient for uint64_t. */
static void factorize(uint64_t n, uint64_t primes[64], uint64_t exps[64], size_t *cnt) {
    size_t c = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            primes[c] = p;
            exps[c] = e;
            ++c;
        }
    }
    if (n > 1) {
        primes[c] = n;
        exps[c] = 1;
        ++c;
    }
    *cnt = c;
}

/* recursively generate all divisors of n using its prime factorisation.
   divisors are stored in out[]. The variable *pos is incremented as values are added. */
static void gen_divisors_rec(uint64_t cur,
                             size_t idx,
                             const uint64_t primes[64],
                             const uint64_t exps[64],
                             size_t fcnt,
                             uint64_t *out,
                             size_t *pos) {
    if (idx == fcnt) {
        out[*pos] = cur;
        ++(*pos);
        return;
    }
    uint64_t val = cur;
    for (size_t e = 0; e <= exps[idx]; ++e) {
        gen_divisors_rec(val, idx + 1, primes, exps, fcnt, out, pos);
        val *= primes[idx];
    }
}

/* main sequence function */
seq_t a(idx_t n) {
    uint64_t N = n;

    /* factorise N */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t fcnt;
    factorize(N, primes, exps, &fcnt);

    /* number of divisors tau = Π (exp + 1) */
    size_t tau = 1;
    for (size_t i = 0; i < fcnt; ++i) {
        tau *= (exps[i] + 1);
    }

    /* allocate divisor array */
    uint64_t *divs = (uint64_t *)malloc(tau * sizeof(uint64_t));
    if (!divs) {
        return (seq_t)0;  /* out‑of‑memory: best effort */
    }

    /* generate all divisors */
    size_t pos = 0;
    gen_divisors_rec(1, 0, primes, exps, fcnt, divs, &pos);

    /* sort divisors ascending */
    qsort(divs, tau, sizeof(uint64_t), cmp_uint64);

    /* compute sum of min(i,j) over all ordered pairs of divisors */
    seq_t sum = 0;
    seq_t prefix = 0;
    for (size_t i = 0; i < tau; ++i) {
        seq_t d = (seq_t)divs[i];
        prefix += d;
        seq_t term = prefix + (seq_t)(tau - 1 - i) * d;
        sum += term;
    }

    free(divs);
    return sum;
}
