/*
 * A057661  a(n) = Sum_{k=1..n} lcm(n,k)/n.
 * https://oeis.org/A057661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = Sum_{k=1..n} lcm(n,k)/n Derived identity used: a(n) =
 *            Σ_{d|n} psi(d), where psi(m) = 1 if m==1 else m*phi(m)/2
 * Range:     u8 n <= 24, u16 n <= 366, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* Factor n into primes: primes[] contains distinct primes,  */
/* exps[] contains their exponents, and *cnt records the count. */
/* ------------------------------------------------------------------ */
static void factor(uint64_t n, uint64_t *primes, int *exps, int *cnt)
{
    *cnt = 0;
    if (n <= 1) return;

    /* factor 2 separately */
    int e = 0;
    while (n % 2ULL == 0ULL) {
        n /= 2ULL;
        ++e;
    }
    if (e > 0) {
        primes[*cnt] = 2ULL;
        exps[*cnt]   = e;
        ++(*cnt);
    }

    /* trial by odd numbers */
    for (uint64_t p = 3ULL; p <= n / p; p += 2ULL) {
        if (n % p == 0ULL) {
            e = 0;
            while (n % p == 0ULL) {
                n /= p;
                ++e;
            }
            primes[*cnt] = p;
            exps[*cnt]   = e;
            ++(*cnt);
        }
    }

    if (n > 1ULL) {
        primes[*cnt] = n;
        exps[*cnt]   = 1;
        ++(*cnt);
    }
}

/* ------------------------------------------------------------------ */
/* Recursive enumeration of all divisors of n.  During recursion
   we keep the current divisor (`cur_d`) and its Euler totient
   (`cur_phi`).  For each divisor, we add its psi value to *sum.  */
/* ------------------------------------------------------------------ */
static void dfs(const uint64_t *primes, const int *exps, int idx,
                int total, uint64_t cur_d, uint64_t cur_phi, seq_t *sum)
{
    if (idx == total) {
        seq_t psi = (cur_d == 1ULL) ? (seq_t)1
                                    : ((seq_t)cur_d * (seq_t)cur_phi) / 2;
        *sum += psi;
        return;
    }

    uint64_t p   = primes[idx];
    int     eMax = exps[idx];

    /* Exponent 0: divisor unchanged, phi unchanged */
    dfs(primes, exps, idx + 1, total, cur_d, cur_phi, sum);

    uint64_t d   = cur_d;
    uint64_t phi = cur_phi;

    for (int e = 1; e <= eMax; ++e) {
        d *= p;
        if (e == 1)
            phi = cur_phi * (p - 1);      /* phi(p) = p-1 */
        else
            phi *= p;                     /* phi(p^e) = phi(p^(e-1))*p */
        dfs(primes, exps, idx + 1, total, d, phi, sum);
    }
}

/* ------------------------------------------------------------------ */
/* Public function to compute a(n) as defined by OEIS A057661.       */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;       /* undefined below offset, safe */

    uint64_t primes_arr[64];
    int     exps_arr[64];
    int      cnt;

    factor((uint64_t)n, primes_arr, exps_arr, &cnt);

    seq_t sum = 0;
    dfs(primes_arr, exps_arr, 0, cnt, (uint64_t)1, (uint64_t)1, &sum);

    return sum;
}
