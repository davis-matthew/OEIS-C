/*
 * A211422  Number of ordered triples (w,x,y) with all terms in
 *          {-n,...,0,...,n} and w^2 + x*y = 0.
 * https://oeis.org/A211422
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n+1 + 4*Σ_{a=1}^{n} f(a) , where f(a) counts positive
 *            divisors d of a^2 with d ≤ n and a^2/d ≤ n
 * Range:     u8 n <= 20, u16 n <= 2745, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Count divisors d of a^2 that satisfy d ≤ n and a^2/d ≤ n. */
static void count_divs(idx_t pos, uint64_t cur, const idx_t *pr, const idx_t *ex,
                       idx_t facCnt, uint64_t a2, idx_t n, uint64_t *cnt)
{
    if (pos == facCnt) {
        if (cur <= (uint64_t)n && a2 / cur <= (uint64_t)n)
            (*cnt)++;
        return;
    }
    uint64_t val = cur;
    for (idx_t e = 0; e <= ex[pos]; ++e) {
        count_divs(pos + 1, val, pr, ex, facCnt, a2, n, cnt);
        if (e == ex[pos]) break;
        val *= pr[pos];
    }
}

seq_t a(idx_t n)
{
    /* Smallest‑prime‑factor sieve, reused for successive calls. */
    static idx_t *spf = NULL;
    static idx_t spf_size = 0;

    if (spf_size <= n) {
        idx_t new_size = n + 1;
        idx_t *new_spf = (idx_t *)malloc(new_size * sizeof(idx_t));
        if (!new_spf) return (seq_t)0;
        for (idx_t i = 0; i < new_size; ++i) new_spf[i] = 0;
        for (idx_t i = 2; i < new_size; ++i) {
            if (new_spf[i] == 0) {
                new_spf[i] = i;
                if (i <= n / i)
                    for (idx_t j = i * i; j < new_size; j += i)
                        if (new_spf[j] == 0)
                            new_spf[j] = i;
            }
        }
        if (spf) free(spf);
        spf = new_spf;
        spf_size = new_size;
    }

    uint64_t result = 4ULL * n + 1ULL;  /* contribution from w = 0 */

    /* Temporary storage for factorization of each a. */
    idx_t primes[20];
    idx_t exps[20];

    for (idx_t a = 1; a <= n; ++a) {
        idx_t x = a;
        int facCnt = 0;
        /* Factor a using the SPF table. */
        while (x > 1) {
            idx_t p = spf[x];
            idx_t e = 0;
            while (x % p == 0) {
                x /= p;
                ++e;
            }
            primes[facCnt] = p;
            exps[facCnt] = e * 2;     /* exponent in a² */
            ++facCnt;
        }

        uint64_t a2 = (uint64_t)a * (uint64_t)a;
        uint64_t cnt = 0;
        count_divs(0, 1ULL, primes, exps, facCnt, a2, n, &cnt);

        result += 4ULL * cnt;   /* two w values times two sign choices for (x,y) */
    }

    return (seq_t)result;
}
