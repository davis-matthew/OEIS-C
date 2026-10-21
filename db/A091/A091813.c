/*
 * A091813  Number of positive squarefree integers k<=n satisfying
 *          gcd_*(k,n)=1, where gcd_*(k,n) is the greatest divisor of k that
 *          is also a unitary divisor of n.
 * https://oeis.org/A091813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of squarefree integers k ≤ n for which
 *            gcd_*(k,n)=1 (i.e., k does not contain any prime divisor of n
 *            that appears with exponent exactly 1 in n). The function uses a
 *            sieve of smallest prime factors and factorises each k up to n.
 *            It works for arbitrary n ≥ 1
 * Range:     u8 n <= 420, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ---- sieve of smallest prime factors ----------------------------------- */
static int *spf = NULL;          /* spf[i] = smallest prime factor of i  */
static int sieve_limit = 0;      /* current maximum n for which spf is valid   */

static void init_sieve(int n)
{
    if (n <= sieve_limit) return;                /* already up to date */

    free(spf);
    spf = (int *)malloc((n + 1) * sizeof(int));
    if (!spf) exit(1);                           /* out of memory */

    for (int i = 0; i <= n; ++i) spf[i] = i;
    for (int i = 2; i * i <= n; ++i) {
        if (spf[i] == i) {                       /* i is prime */
            for (int j = i * i; j <= n; j += i)
                if (spf[j] == j) spf[j] = i;
        }
    }
    sieve_limit = n;
}

/* ---- helper to check if k is squarefree and coprime to the
         primes that occur once in n ----------------------------- */
static int squarefree_and_coprime(int k, const int *sPrimes, int sCnt)
{
    int x = k;
    while (x > 1) {
        int p = spf[x];
        x /= p;                      /* remove one factor p */
        if (x % p == 0) return 0;    /* repeated prime → not squarefree */
        /* test whether p is forbidden */
        for (int i = 0; i < sCnt; ++i)
            if (sPrimes[i] == p) return 0;
    }
    return 1;                          /* squarefree and coprime to sPrimes */
}

/* ---- main sequence function ------------------------------------------- */
seq_t a(idx_t n)
{
    /* prepare sieve up to n */
    init_sieve((int)n);

    /* factor n and collect primes that appear with exponent exactly 1 */
    int tmp = (int)n;
    int sPrimes[32];   /* sufficient for all n up to 2^31-1 */
    int sCnt = 0;
    while (tmp > 1) {
        int p = spf[tmp], e = 0;
        while (tmp % p == 0) { tmp /= p; ++e; }
        if (e == 1) sPrimes[sCnt++] = p;
    }

    /* count suitable k */
    seq_t count = 0;
    for (idx_t k = 1; k <= n; ++k)
        if (squarefree_and_coprime((int)k, sPrimes, sCnt))
            ++count;

    return count;
}
