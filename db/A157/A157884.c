/*
 * A157884  For each positive integer m there exist at least one prime Q=Q(m)
 *          and at least one prime P=P(m) such that (2m-1)^2 < Q < (2m)^2 -
 *          (2m-1) <= P < (2m)^2. Sequence lists pairs P(m), Q(m) for m >= 1.
 *          If more than one prime for P or Q exists, we take the smallest.
 * https://oeis.org/A157884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A157884: for each positive integer m
   find the smallest prime Q in (2m−1)² < Q < (2m)²−(2m−1)
   and the smallest prime P in (2m)²−(2m−1) ≤ P < (2m)².
   The sequence outputs Q(m) and P(m) alternately:
   a(1)=Q(1), a(2)=P(1), a(3)=Q(2), … */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* ---------- modular arithmetic helpers ---------- */
static inline uint64_t mulmod(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

static inline uint64_t powmod(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1;
    while (e)
    {
        if (e & 1) r = mulmod(r, a, mod);
        a = mulmod(a, a, mod);
        e >>= 1;
    }
    return r;
}

/* ---------- deterministic prime test for 64‑bit numbers ---------- */
static bool is_prime_64(idx_t n)
{
    if (n < 2) return false;
    static const uint32_t smallPrimes[] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47};
    size_t i;
    for (i = 0; i < sizeof smallPrimes/sizeof smallPrimes[0]; ++i)
    {
        uint32_t p = smallPrimes[i];
        if ((uint64_t)n == p) return true;
        if (n % p == 0) return false;
    }

    /* Miller‑Rabin with bases 2, 7, 61 – deterministic for n < 2,152,302,898,747 */
    idx_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    static const uint32_t bases[] = {2,7,61};
    for (i = 0; i < sizeof bases/sizeof bases[0]; ++i)
    {
        uint32_t a = bases[i];
        if (a % n == 0) continue;
        uint64_t x = powmod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool ok = false;
        int r;
        for (r = 1; r < s; ++r)
        {
            x = mulmod(x, x, n);
            if (x == n - 1) { ok = true; break; }
        }
        if (!ok) return false;
    }
    return true;
}

/* ---------- smallest prime in [lo,hi_excl) ---------- */
static idx_t next_prime_in_range(idx_t lo, idx_t hi_excl)
{
    if (lo <= 2 && 2 < hi_excl) return 2;
    if (lo < 3) lo = 3;
    if ((lo & 1) == 0) lo++;          /* start with odd */
    for (idx_t x = lo; x < hi_excl; x += 2)
    {
        if (is_prime_64(x)) return x;
    }
    return 0;    /* should never happen for valid input */
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, harmless */

    /* m = ceil(n/2) = (n+1)/2 */
    idx_t m = (n + 1) >> 1;

    idx_t two_m    = m << 1;           /* 2m */
    idx_t sq2m     = two_m * two_m;    /* (2m)^2 */

    idx_t lo_q = ((two_m - 1) * (two_m - 1)) + 1;   /* (2m-1)^2 + 1   */
    idx_t hi_q = sq2m - (two_m - 1);                 /* (2m)^2 - (2m-1) */
    idx_t lo_p = hi_q;           /* inclusive lower bound for P(m) */
    idx_t hi_p = sq2m;           /* exclusive upper bound for P(m) */

    idx_t result;
    if ((n & 1) == 1)          /* odd index → Q(m) */
        result = next_prime_in_range(lo_q, hi_q);
    else                       /* even index → P(m) */
        result = next_prime_in_range(lo_p, hi_p);

    return (seq_t)result;
}
