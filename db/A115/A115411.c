/*
 * A115411  a(n) = least k such that semiprime(n) divides k-th triangular
 *          number.
 * https://oeis.org/A115411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A115411:
   a(n) = MIN{k | semiprime(n) divides k(k+1)/2}
   (semiprime(n) is the nth product of two primes, counting multiplicity)
   A semiprime p may be even or odd.  For even p = 2·q the value can be
   obtained in closed form:
     if p ≡ 2 (mod 8)   →   a(n) = p/2 – 1
     if p ≡ 6 (mod 8)   →   a(n) = p/2
   For all other p the minimum k is found by solving the congruences
   k ≡ 0 or –1 (mod qᵉ) for every prime power qᵉ dividing 2·p.
   The solution is the smallest positive integer satisfying all these
   simultaneous congruences.  This is done by a brute‑force search over
   all 2ⁿ possibilities (n ≤ 3 for a semiprime) using the Chinese
   Remainder Theorem.  All arithmetic is performed in 64‑bit unsigned
   integers, which is sufficient for the limits required (n ≤ 10 000). */

#include <stdint.h>
#include <stdlib.h>

/* ---------- helper: extended gcd for 64‑bit integers ---------- */
static int64_t egcd(int64_t a, int64_t b, int64_t *x, int64_t *y)
{
    if (b == 0) {
        *x = 1;
        *y = 0;
        return a;
    }
    int64_t x1, y1;
    int64_t g = egcd(b, a % b, &x1, &y1);
    *x = y1;
    *y = x1 - (a / b) * y1;
    return g;
}

/* modular inverse of a modulo m (m > 1, a and m coprime) */
static uint64_t modinv(uint64_t a, uint64_t m)
{
    int64_t x, y;
    egcd((int64_t)a, (int64_t)m, &x, &y);
    int64_t inv = x % (int64_t)m;
    if (inv < 0)
        inv += m;
    return (uint64_t)inv;
}

/* ---------- data kept between calls ---------- */
static uint64_t *spf = NULL;            /* smallest prime factor array   */
static size_t   spf_limit = 0;          /* size of spf array (spf_limit+1) */
static uint64_t *semiprimes = NULL;     /* list of semiprimes (uint64_t)  */
static size_t   semiprime_cnt = 0;      /* number of semiprimes stored    */
static seq_t   *cache = NULL;           /* memoised a(n) values           */
static size_t   cache_cap = 0;          /* size of cache array            */

/* ---------- compute spf up to a given limit ---------- */
static void compute_spf(size_t limit)
{
    spf = (uint64_t *)realloc(spf, (limit + 1) * sizeof(uint64_t));
    for (size_t i = 0; i <= limit; ++i)
        spf[i] = 0;

    for (size_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {         /* i is prime */
            spf[i] = i;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0)
                        spf[j] = i;
            }
        }
    }
    spf_limit = limit;
}

/* ---------- fill semiprimes up to at least 'need' values ---------- */
static void ensure_semiprimes(idx_t need)
{
    /* if we already have enough, nothing to do */
    if (semiprime_cnt >= (size_t)need)
        return;

    /* start with a modest limit and double until sufficient */
    size_t limit = spf_limit ? spf_limit * 2 : 1000;
    while (1) {
        compute_spf(limit);

        /* build list of semiprimes up to 'limit' */
        uint64_t *sp_list = (uint64_t *)malloc((limit / 2 + 1) * sizeof(uint64_t));
        size_t idx = 0;
        for (size_t n = 2; n <= limit; ++n) {
            size_t m = n, cnt = 0;
            while (m > 1 && cnt <= 2) {
                uint64_t p = spf[m];
                while (m % p == 0) {
                    ++cnt;
                    m /= p;
                    if (cnt > 2)
                        break;
                }
            }
            if (cnt == 2)
                sp_list[idx++] = (uint64_t)n;
        }

        /* if this limit gives enough semiprimes, keep it */
        if (idx >= (size_t)need) {
            free(semiprimes);
            semiprimes = sp_list;
            semiprime_cnt = idx;

            /* grow cache if needed */
            if (cache_cap < semiprime_cnt) {
                cache = (seq_t *)realloc(cache,
                                          semiprime_cnt * sizeof(seq_t));
                /* initialise new positions to 0 */
                for (size_t i = cache_cap; i < semiprime_cnt; ++i)
                    cache[i] = 0;
                cache_cap = semiprime_cnt;
            }
            return;
        }

        /* not enough, try a larger limit */
        free(sp_list);
        limit <<= 1;
    }
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined, but keep safe */

    /* make sure the required semiprime exists */
    ensure_semiprimes(n);
    size_t idx = (size_t)(n - 1);

    /* use cached value if already computed */
    if (cache[idx] != 0)
        return cache[idx];

    uint64_t p = semiprimes[idx];   /* nth semiprime, fits in 64‑bit */

    /* special‑case formula for even semiprimes (p = 2 · q) */
    if ((p & 1) == 0) {
        uint64_t r = p % 8;
        if (r == 2) {
            cache[idx] = (seq_t)(p / 2 - 1);
            return cache[idx];
        }
        if (r == 6) {
            cache[idx] = (seq_t)(p / 2);
            return cache[idx];
        }
        /* even semiprime with p%8==0 or 4 – fall through to general case */
    }

    /* factor p using spf (p <= spf_limit) */
    uint64_t tmp = p;
    uint64_t fac_pr[3] = {0, 0, 0};
    int    fac_ex[3]  = {0, 0, 0};
    int    nf         = 0;
    while (tmp > 1) {
        uint64_t prime = spf[tmp];
        int e = 0;
        while (tmp % prime == 0) {
            ++e;
            tmp /= prime;
        }
        fac_pr[nf] = prime;
        fac_ex[nf] = e;
        ++nf;
    }

    /* add factor 2 coming from the factor 2 in 2·p */
    int has2 = -1;
    for (int i = 0; i < nf; ++i)
        if (fac_pr[i] == 2) {
            has2 = i;
            break;
        }
    if (has2 == -1) {
        fac_pr[nf] = 2;
        fac_ex[nf] = 1;
        ++nf;
    } else {
        fac_ex[has2] += 1;
    }

    /* build modulus array: q^e for each prime power */
    uint64_t mods[3];
    for (int i = 0; i < nf; ++i) {
        uint64_t m = 1;
        for (int j = 0; j < fac_ex[i]; ++j)
            m *= fac_pr[i];
        mods[i] = m;
    }

    /* enumerate all 2^nf residue combinations */
    uint64_t best = (uint64_t)-1;  /* large placeholder */
    for (uint64_t mask = 0; mask < ((uint64_t)1 << nf); ++mask) {
        uint64_t res = 0;          /* CRT solution so far   */
        uint64_t curMod = 1;       /* modulus so far        */

        for (int i = 0; i < nf; ++i) {
            uint64_t residue = ((mask >> i) & 1ULL) ? mods[i] - 1 : 0;
            if (curMod == 1) {
                res = residue;
                curMod = mods[i];
                continue;
            }
            /* combine with next modulus */
            uint64_t rem = curMod % mods[i];
            uint64_t delta;
            uint64_t rup = res % mods[i];
            if (residue >= rup)
                delta = residue - rup;
            else
                delta = residue + mods[i] - rup;

            uint64_t inv = modinv(curMod % mods[i], mods[i]);
            uint64_t t = (delta * inv) % mods[i];

            res = res + curMod * t;
            curMod = curMod * mods[i];
            res %= curMod;
        }

        uint64_t candidate = res % curMod;
        if (candidate == 0)
            candidate = curMod;  /* k must be positive */

        if (candidate < best)
            best = candidate;
    }

    cache[idx] = (seq_t)best;
    return cache[idx];
}
