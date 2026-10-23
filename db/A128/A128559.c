/*
 * A128559  a(n) is the smallest positive integer k such that n!/k! has at
 *          most n divisors.
 * https://oeis.org/A128559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A128559
 *  a(n) = smallest k (1 ≤ k ≤ n) such that n! / k! has at most n divisors.
 *  The function works for any integer type seq_t (unsigned 8‑…, 64‑bit,
 *  __int128 or arbitrary‑precision class) and idx_t (uint64_t).  It builds
 *  smallest‑prime‑factor tables lazily and recomputes the divisor count
 *  from n down to the answer.  No assumptions are made about the order of
 *  calls.  The implementation uses only integer arithmetic allowed by the
 *  specification and is valid in C11 and C++17.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* -------------  Prime tables  ---------------------------------- */
    static idx_t spf_max = 0;           /* largest n for which spf is built */
    static int  *spf      = NULL;       /* smallest prime factor for i */
    static int  *prime_idx= NULL;       /* prime → index in primes array */
    static uint32_t *primes = NULL;     /* list of primes up to spf_max   */
    static size_t   primes_cnt = 0;     /* number of primes                */

    if (n > spf_max) {
        /* --- build spf up to n ----------------------------------- */
        int *new_spf = (int*)malloc((n + 1) * sizeof(int));
        for (idx_t i = 0; i <= n; ++i) new_spf[i] = (int)i;
        for (idx_t i = 2; i * i <= n; ++i) {
            if (new_spf[i] == (int)i) {
                for (idx_t j = i * i; j <= n; j += i)
                    if (new_spf[j] == (int)j) new_spf[j] = (int)i;
            }
        }

        /* --- build primes list and mapping ----------------------- */
        size_t pc = 0;
        for (idx_t i = 2; i <= n; ++i)
            if (new_spf[i] == (int)i) ++pc;

        uint32_t *new_primes  = (uint32_t*)malloc(pc * sizeof(uint32_t));
        int       *new_pidx   = (int*)   malloc((n + 1) * sizeof(int));
        for (idx_t i = 0; i <= n; ++i) new_pidx[i] = -1;
        size_t pi = 0;
        for (idx_t i = 2; i <= n; ++i) {
            if (new_spf[i] == (int)i) {
                new_primes[pi] = (uint32_t)i;
                new_pidx[i]    = (int)pi;
                ++pi;
            }
        }

        /* --- replace old tables ----------------------------------- */
        free(spf);   free(prime_idx); free(primes);
        spf      = new_spf;
        prime_idx= new_pidx;
        primes   = new_primes;
        primes_cnt= pc;
        spf_max  = n;
    }

    /* -------------  Divisor count loop  --------------------------- */
    int *exp = (int*)calloc(primes_cnt, sizeof(int)); /* exponents of primes */
    seq_t divcnt = 1;                                 /* divisor count */

    idx_t k = n;
    while (true) {
        /* Divisor count too large → answer is k+1 */
        if (divcnt > (seq_t)n) {
            seq_t ans = (seq_t)(k + 1);
            free(exp);
            return ans;
        }

        /* Reached k==1 → answer is 1 */
        if (k == 1) {
            free(exp);
            return 1;
        }

        /* Add factor k to the product and update exponents */
        idx_t val = k;
        while (val > 1) {
            int p = spf[val];
            int e = 0;
            while (val % p == 0) {
                val /= p;
                ++e;
            }
            int idxp  = prime_idx[p];
            int old   = exp[idxp];
            int neu   = old + e;
            exp[idxp] = neu;
            divcnt /= (seq_t)(old + 1);
            divcnt *= (seq_t)(neu + 1);
        }
        --k;
    }

    /* unreachable, but keep compiler happy */
    free(exp);
    return 0;
}
