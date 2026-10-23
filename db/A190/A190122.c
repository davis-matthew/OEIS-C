/*
 * A190122  a(n) = Sum_{k=1..n} k*lcm(k,k')/gcd(k,k'), where k' is arithmetic
 *          derivative of k.
 * https://oeis.org/A190122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k*lcm(k,k')/gcd(k,k')
 * Range:     u8 n <= 6, u16 n <= 32, u32 n <= 529, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where k' is the arithmetic derivative of k */
/* Supports arbitrary order of calls; memoises partial sums */
/* Requires the gcd(seq_t,seq_t) function from utils/gcd.h */

#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* static cache of partial sums: pref[0] = 0, pref[i] = a(i) */
    static seq_t *pref = NULL;
    static idx_t pref_size = 0;          /* highest index stored in pref */

    /* static cache of smallest prime factors up to current bound */
    static uint32_t *spf = NULL;
    static idx_t spf_size = 0;           /* maximum n for which spf has been built */

    /* ensure sieve covers up to n */
    if (n > spf_size)
    {
        /* allocate new sieve array */
        uint32_t *newspf = (uint32_t *)calloc((size_t)(n + 1), sizeof(uint32_t));
        for (idx_t i = 2; i <= n; ++i)
            if (newspf[i] == 0)
            {
                newspf[i] = (uint32_t)i;
                if (i * i <= n)
                    for (idx_t j = i * i; j <= n; j += i)
                        if (newspf[j] == 0)
                            newspf[j] = (uint32_t)i;
            }
        free(spf);
        spf = newspf;
        spf_size = n;
    }

    /* compute pref[1..n] if needed */
    if (n > pref_size)
    {
        size_t newsize = (size_t)(n + 1);
        seq_t *newpref = (seq_t *)realloc(pref, newsize * sizeof(seq_t));
        if (!newpref) /* allocation failure – safest return */
            return 0;
        pref = newpref;
        if (pref_size == 0)
            pref[0] = 0;

        /* start computing from pref_size+1 to n */
        for (idx_t k = pref_size + 1; k <= n; ++k)
        {
            /* arithmetic derivative k' */
            idx_t kk = k;
            seq_t kp = 0;          /* k' */
            while (kk > 1)
            {
                idx_t p = spf[kk];
                idx_t e = 0;
                while (kk % p == 0)
                {
                    kk /= p;
                    ++e;
                }
                kp += (seq_t)e * (seq_t)(k / p);
            }

            /* gcd(k, k') */
            seq_t d = gcd((seq_t)k, kp);
            seq_t d2 = d * d;

            /* term = k^2 * k' / d^2 */
            seq_t term = ((seq_t)k * (seq_t)k * kp) / d2;

            pref[k] = pref[k - 1] + term;
        }
        pref_size = n;
    }

    return pref[n];
}
