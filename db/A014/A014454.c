/*
 * A014454  Sum_{1<=k<n} gcd(k!,n!/k!).
 * https://oeis.org/A014454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{1 <= k < n} gcd( k! , n!/k! )
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 25, u64 n <= 39, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Fast exponentiation for seq_t values. */
static inline seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Compute the sequence term. */
seq_t a(idx_t n) {
    if (n <= 1) return 0;

    /* 1. Build smallest prime factor array for 2..n. */
    idx_t *spf = (idx_t*)malloc((n + 1) * sizeof(idx_t));
    for (idx_t i = 0; i <= n; ++i) spf[i] = 0;
    for (idx_t i = 2; i <= n; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (i * i <= n)
                for (idx_t j = i * i; j <= n; j += i)
                    if (spf[j] == 0) spf[j] = i;
        }
    }

    /* 2. Collect primes and pre‑compute v_p(n!) for each prime. */
    idx_t *primes = (idx_t*)malloc((n + 1) * sizeof(idx_t));
    idx_t prime_cnt = 0;
    uint32_t *exp_n = (uint32_t*)calloc((n + 1), sizeof(uint32_t)); /* indexed by prime value */
    for (idx_t i = 2; i <= n; ++i) {
        if (spf[i] == i) {               /* i is prime */
            primes[prime_cnt++] = i;
            /* Legendre formula for v_p(n!) */
            uint32_t e = 0;
            idx_t power = i;
            while (power <= n) {
                e += n / power;
                power *= i;
            }
            exp_n[i] = e;
        }
    }

    /* 3. Current exponents of primes in k!. */
    uint32_t *exp_cur = (uint32_t*)calloc((n + 1), sizeof(uint32_t));
    /* 4. List of primes that have become non‑zero in exp_cur. */
    idx_t *seen = (idx_t*)malloc((n + 1) * sizeof(idx_t));
    idx_t seen_cnt = 0;

    seq_t total = 0;

    /* Main loop over k = 1 .. n-1 */
    for (idx_t k = 1; k < n; ++k) {
        /* Factorize k and update exp_cur. */
        idx_t x = k;
        while (x > 1) {
            idx_t p = spf[x];
            idx_t cnt = 0;
            while (x % p == 0) {
                x /= p;
                ++cnt;
            }
            if (exp_cur[p] == 0) {
                seen[seen_cnt++] = p;
            }
            exp_cur[p] += cnt;
        }

        /* Compute gcd(k!, n!/k!) = product over primes in seen of p^min(exp_cur[p], exp_n[p]-exp_cur[p]) */
        seq_t g = 1;
        for (idx_t i = 0; i < seen_cnt; ++i) {
            idx_t p = seen[i];
            uint32_t cur = exp_cur[p];
            uint32_t rem = exp_n[p] - cur;
            uint32_t m = cur <= rem ? cur : rem;
            if (m == 0) continue;
            g *= pow_seq((seq_t)p, (idx_t)m);
        }
        total += g;
    }

    /* Clean up */
    free(spf);
    free(primes);
    free(exp_n);
    free(exp_cur);
    free(seen);

    return total;
}
