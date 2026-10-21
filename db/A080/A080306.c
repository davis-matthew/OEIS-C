/*
 * A080306  Numerator of Sum(k^mu(k): 1<=k<=n), where mu is the Moebius
 *          function (A008683).
 * https://oeis.org/A080306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of Σ_{k=1}^{n} k^μ(k) where μ is the Möbius
 *            function
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 22, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* Smallest prime factor tables                                         */
/* ------------------------------------------------------------------ */
static uint64_t *spf = NULL;     /* spf[i] = smallest prime factor of i (i>=2) */
static idx_t    spf_size = 0;    /* allocated size of spf array              */

/* Build/spawn smallest‑prime‑factor table up to limit (inclusive). */
static void init_spf(idx_t limit)
{
    if (limit <= spf_size)
        return;                /* already computed */

    free(spf);
    spf = (uint64_t*)malloc((size_t)(limit + 1) * sizeof(uint64_t));
    if (!spf)
        return;                /* allocation failed – leave array empty */

    spf_size = limit;
    for (idx_t i = 0; i <= limit; ++i)
        spf[i] = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {                 /* i is prime            */
            spf[i] = i;
            if (i <= limit / i) {
                for (idx_t j = i * i; j <= limit; j += i) {
                    if (spf[j] == 0)
                        spf[j] = i;
                }
            }
        }
    }
}

/* ------------------------------------------------------------------ */
/* Auxiliary arithmetic                                                 */
/* ------------------------------------------------------------------ */

/* Greatest common divisor for seq_t.  Works for all supported types. */
static seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* ------------------------------------------------------------------ */
/* Möbius function (uses precomputed SPF table)                        */
/* ------------------------------------------------------------------ */
static int mobius(idx_t n)
{
    if (n <= 1)          /* μ(1) == 1 */
        return 1;

    int cnt = 0;                  /* number of distinct prime factors */
    idx_t m = n;
    while (m > 1) {
        uint64_t p = spf[m];
        if (p == 0)                 /* m is prime if spf not set (should not happen) */
            p = m;
        int exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
            if (exp > 1)
                return 0;          /* not square‑free */
        }
        ++cnt;
    }
    return (cnt & 1) ? -1 : 1; /* μ(n) = (-1)^cnt for square‑free n */
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                              */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined below offset – return 0 */

    init_spf(n);          /* ensure SPF table exists up to n */

    seq_t num = 0;        /* current numerator */
    seq_t den = 1;        /* current denominator */

    for (idx_t k = 1; k <= n; ++k) {
        int mu = mobius(k);

        seq_t tnum, tden;
        if (mu == 1) {                 /* term = k / 1 */
            tnum = (seq_t)k;
            tden = (seq_t)1;
        } else if (mu == -1) {         /* term = 1 / k */
            tnum = (seq_t)1;
            tden = (seq_t)k;
        } else {                       /* mu == 0 -> term = 1 / 1 */
            tnum = (seq_t)1;
            tden = (seq_t)1;
        }

        /* Add tnum/tden to num/den */
        seq_t new_num = num * tden + den * tnum;
        seq_t new_den = den * tden;

        seq_t g = seq_gcd(new_num, new_den);
        num = new_num / g;
        den = new_den / g;
    }

    return num;        /* numerator of the reduced sum */
}
