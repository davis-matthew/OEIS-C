/*
 * A051493  Triangles with perimeter n and relatively prime integer side
 *          lengths.
 * https://oeis.org/A051493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} μ(d)·A005044(n/d), where A005044(m) =
 *            round(m²/48) for even m, round((m+3)²/48) for odd m
 * Range:     u8 n <= 108, u16 n <= 1776, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 90 terms through n = 9697 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Recursively generate all divisors of `n` together with their Möbius value.  */
static void
gen_divs_with_mu(const uint64_t *primes,
                 const uint64_t *exps,
                 size_t        prime_cnt,
                 size_t        idx,
                 uint64_t      cur,
                 int           mu_val,
                 int           has_square,
                 uint64_t     *divs,
                 int           *mu_vals,
                 size_t       *cnt)
{
    if (idx == prime_cnt) {
        divs[*cnt]   = cur;
        mu_vals[*cnt] = has_square ? 0 : mu_val;
        ++(*cnt);
        return;
    }
    uint64_t p = primes[idx];
    uint64_t val = cur;
    for (uint64_t e = 0; e <= exps[idx]; ++e) {
        int n_mu = mu_val;
        int n_sq = has_square;
        if (e == 1)          /* first power of prime */
            n_mu = -n_mu;
        else if (e > 1)      /* introduces a square factor */
            n_sq = 1;
        gen_divs_with_mu(primes, exps, prime_cnt,
                         idx + 1, val, n_mu, n_sq,
                         divs, mu_vals, cnt);
        val *= p;
    }
}

/* Number of integer triangles with perimeter `perim`.  This is A005044. */
static seq_t triangles_of_perimeter(idx_t perim)
{
    /* Use 64‑bit intermediates to avoid overflow for small seq_t sizes. */
    uint64_t x;
    if ((perim & 1) == 0) {          /* even perimeter */
        x = (uint64_t)perim * (uint64_t)perim;   /* m²  */
    } else {                         /* odd perimeter */
        uint64_t t = perim + 3;
        x = t * t;                     /* (m+3)² */
    }
    /* Round: (x + 24) / 48  (48/2 = 24).  Integer division truncates. */
    return (seq_t)((x + 24ULL) / 48ULL);
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* Factor `n`.  Use at most 16 distinct primes for n ≤ 10⁴. */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t pcnt = 0;
    uint64_t temp = (uint64_t)n;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[pcnt]   = p;
            exps[pcnt]     = e;
            ++pcnt;
        }
    }
    if (temp > 1) {            /* remaining prime factor */
        primes[pcnt] = temp;
        exps[pcnt]   = 1;
        ++pcnt;
    }

    /* Generate all divisors and their Möbius μ value. */
    uint64_t divs[128];
    int       mu_vals[128];
    size_t    dcnt = 0;
    gen_divs_with_mu(primes, exps, pcnt,
                     0, 1ULL, 1, 0,
                     divs, mu_vals, &dcnt);

    /* Sum over divisors: μ(d) * A005044(n/d). */
    long long sum = 0;
    for (size_t i = 0; i < dcnt; ++i) {
        uint64_t d = divs[i];
        int mu = mu_vals[i];
        if (mu == 0)
            continue;
        idx_t k = n / d;
        seq_t t = triangles_of_perimeter(k);
        if (mu > 0)
            sum += (long long)t;
        else
            sum -= (long long)t;
    }
    return (seq_t)sum;
}
