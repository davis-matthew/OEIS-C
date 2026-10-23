/*
 * A023840  Sum of exponents in prime-power factorization of C(5n,n-3).
 * https://oeis.org/A023840
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of exponents in the prime‑power factorization of
 *            C(5n, n-3)
 * Range:     u8 n <= 673, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper: compute all primes ≤ limit and return them in a newly allocated array.
   The caller owns the returned array and must free it. */
static void compute_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out)
{
    if (limit < 2) {
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }
    /* Sieve of Eratosthenes – use a byte array for the bit map */
    unsigned char *mark = (unsigned char *)calloc((size_t)(limit + 1), sizeof(unsigned char));
    if (!mark) return; /* allocation failure – should never happen for limits ≤ 5×10⁴ */

    mark[0] = mark[1] = 1;          /* 0 and 1 are not prime */
    idx_t i, j;
    for (i = 2; i * i <= limit; ++i) {
        if (!mark[i]) {
            for (j = i * i; j <= limit; j += i)
                mark[j] = 1;
        }
    }

    size_t cnt = 0;
    for (i = 2; i <= limit; ++i)
        if (!mark[i]) ++cnt;

    uint64_t *primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!primes) { free(mark); return; }

    size_t pos = 0;
    for (i = 2; i <= limit; ++i)
        if (!mark[i])
            primes[pos++] = (uint64_t)i;

    free(mark);
    *primes_out = primes;
    *cnt_out = cnt;
}

/* ---------------------------------------------------------------------- */
/* Main sequence function                                               */
/* ---------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 3) return 0;      /* value below the offset is irrelevant */

    /* 5n, n-3, and 4n+3 are all ≤ 5·n for n≥3, so their factorials fit
       comfortably in 64‑bit bounds for the test range. */
    const idx_t limit5 = 5 * n;           /* upper bound: 5n              */
    const idx_t limit1 = n - 3;           /* upper bound: n-3              */
    const idx_t limit2 = 4 * n + 3;       /* upper bound: 4n+3             */

    /* ------------------------------------------------------------------ */
    /* Cache the primes up to the largest 5n seen so far.  The cache
       is reused for arbitrary call order, keeping the function fast.     */
    /* ------------------------------------------------------------------ */
    static uint64_t *prime_tab = NULL;
    static size_t    prime_cnt  = 0;
    static idx_t     prime_lim  = 0;

    if (limit5 > prime_lim) {
        /* Need more primes – release old data and recompute */
        free(prime_tab);
        prime_tab = NULL;
        prime_cnt = 0;
        prime_lim = 0;

        compute_primes_upto(limit5, &prime_tab, &prime_cnt);
        prime_lim = limit5;
    }

    /* ------------------------------------------------------------------ */
    /* Compute Ω(C(5n, n-3)) via Legendre's formula.  For each prime p
       ≤ 5n, the exponent in the binomial equals:
           e(p) = Σfloor(5n/pᵏ) – Σfloor((n‑3)/pᵏ) – Σfloor((4n+3)/pᵏ)
    /* ------------------------------------------------------------------ */
    seq_t sum = 0;

    for (size_t idx = 0; idx < prime_cnt; ++idx) {
        const idx_t p = (idx_t)prime_tab[idx];
        if (p > limit5) break;

        idx_t power = p;
        while (power <= limit5) {
            const idx_t term5 = limit5 / power;
            const idx_t term1 = limit1 / power;
            const idx_t term2 = limit2 / power;
            sum += (seq_t)(term5 - term1 - term2);

            /* Advance power; stop if the next multiplication would overflow
               or exceed the bound. */
            if (power > limit5 / p) break;
            power *= p;
        }
    }
    return sum;
}
