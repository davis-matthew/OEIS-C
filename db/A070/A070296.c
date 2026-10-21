/*
 * A070296  Let sphi(k) = number of primes less than k and coprime to it
 *          (A048865); then a(n) = number of integers m with sphi(m) = n.
 * https://oeis.org/A070296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integers m such that A048865(m) = n (OEIS
 *            A070296) – implemented with a sieve and a counter array
 * Range:     u8 none, u16 none, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* The integer types seq_t and idx_t are supplied by the build. */
seq_t a(idx_t n)
{
    /* lazy static initialization --------------------------------------- */
    static int initialized = 0;

    /* upper limit for the sieve – covers values of sphi(m) up to ~200 000 */
    const idx_t LIMIT = 2000000;

    /* arrays used by the sieve */
    static idx_t *is_prime = NULL;
    static idx_t *spf     = NULL;
    static idx_t *pi      = NULL;

    /* array that stores how many m have a given sphi(m) value */
    static seq_t *counts  = NULL;
    static idx_t counts_size = 0;      /* current size of counts array */

    if (!initialized)
    {
        /* --- allocate the sieve tables -------------------------------- */
        is_prime = (idx_t*)malloc((size_t)(LIMIT + 1) * sizeof(idx_t));
        spf     = (idx_t*)malloc((size_t)(LIMIT + 1) * sizeof(idx_t));
        pi      = (idx_t*)malloc((size_t)(LIMIT + 1) * sizeof(idx_t));

        if (!is_prime || !spf || !pi) return 0;  /* allocation failure */

        /* initialize is_prime and spf */
        for (idx_t i = 0; i <= LIMIT; ++i) {
            is_prime[i] = 1;
            spf[i] = 0;
        }
        is_prime[0] = is_prime[1] = 0;

        /* sieve of Eratosthenes with smallest prime factor information */
        for (idx_t i = 2; (idx_t)i * i <= LIMIT; ++i)
            if (is_prime[i])
                for (idx_t j = i * i; j <= LIMIT; j += i) {
                    if (is_prime[j]) {
                        is_prime[j] = 0;
                        if (spf[j] == 0) spf[j] = i;
                    }
                }
        for (idx_t i = 2; i <= LIMIT; ++i)
            if (spf[i] == 0) spf[i] = i;   /* i is prime */

        /* compute prime counting function pi[i] = number of primes ≤ i */
        pi[0] = 0;
        for (idx_t i = 1; i <= LIMIT; ++i)
            pi[i] = pi[i-1] + (is_prime[i] ? 1 : 0);

        /* --- allocate counts array ------------------------------------ */
        counts_size = 128;   /* start small */
        counts = (seq_t*)calloc((size_t)counts_size, sizeof(seq_t));
        if (!counts) return 0;   /* allocation failure */

        /* --- count sphi(m) values ------------------------------------- */
        for (idx_t m = 1; m <= LIMIT; ++m)
        {
            seq_t val;
            if (m == 1) {
                val = 0;
            } else if (is_prime[m]) {
                val = (seq_t)pi[m-1];
            } else {
                /* count distinct prime divisors of m */
                idx_t tmp = m;
                idx_t last = 0;
                seq_t cnt = 0;
                while (tmp > 1) {
                    idx_t p = spf[tmp];
                    if (p != last) { ++cnt; last = p; }
                    while (tmp % p == 0) tmp /= p;
                }
                val = (seq_t)pi[m-1] - cnt;   /* sphi(m) = #primes < m – ω(m) */
            }

            /* enlarge counts array if necessary (val fits in idx_t) */
            idx_t idx = (idx_t)val;   /* val is non‑negative and < LIMIT  */
            if (idx >= counts_size)
            {
                idx_t new_size = idx + 1;
                seq_t *tmp = (seq_t*)realloc(counts, (size_t)new_size * sizeof(seq_t));
                if (!tmp) return 0;   /* allocation failure */
                /* zero-initialize the new part */
                for (idx_t i = counts_size; i < new_size; ++i) tmp[i] = 0;
                counts = tmp;
                counts_size = new_size;
            }
            ++counts[(size_t)idx];
        }

        initialized = 1;
    }

    /* --- answer the query ------------------------------------------- */
    if (n < counts_size) return counts[(size_t)n];
    return 0;
}
