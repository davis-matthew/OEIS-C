/*
 * A216626  Square array read by antidiagonals, T(n,k) = sum_{c|n,d|k}
 *          lcm(c,d) for n>=1, k>=1.
 * https://oeis.org/A216626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 219, u16 n <= 9391, u32 n <= 9391, u64 n <= 9391,
 *            u128 n <= 9391, big any n
 * Helpers:   prime_contrib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* SEQ: A216626 (square array read by antidiagonals)
   Formula: a(n) = T(i,j)
   where
     T(i,j) = Σ_{c|i} Σ_{d|j} lcm(c,d),
   and (i,j) are the coordinates on the antidiagonal that contains
   the index n.  The mapping from n to (i,j) is described in the code
   below.  The implementation uses a small SPF sieve to factor i and j
   and then evaluates T(i,j) by multiplicativity. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/prime_contrib.h"

/* ---------- Integer square root (floor) ---------- */
static idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0, high = x, ans = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid <= x / mid) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

/* ---------- Smallest prime factor sieve ---------- */
static uint32_t *spf = NULL;      /* prime factors up to current limit */
static idx_t   spf_limit = 0;     /* highest index fully sieved */

static void build_spf(idx_t limit)
{
    if (limit <= spf_limit) return;
    if (spf) free(spf);
    spf = (uint32_t *)calloc((size_t)(limit + 1), sizeof(uint32_t));
    if (!spf) abort();

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            for (idx_t j = i * i; j <= limit; j += i) {
                if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }
    for (idx_t i = 2; i <= limit; ++i)
        if (spf[i] == 0) spf[i] = (uint32_t)i;
    spf[1] = 1;
    spf_limit = limit;
}

/* ---------- Compute T(i,j) ---------- */
static seq_t T_value(idx_t i, idx_t j)
{
    /* factorise i */
    uint32_t primesI[16], expsI[16];
    size_t cntI = 0;
    idx_t x = i;
    while (x > 1) {
        uint32_t p = spf[x];
        uint32_t e = 0;
        while (x % p == 0) { x /= p; ++e; }
        primesI[cntI] = p;
        expsI[cntI]   = e;
        ++cntI;
    }

    /* factorise j */
    uint32_t primesJ[16], expsJ[16];
    size_t cntJ = 0;
    x = j;
    while (x > 1) {
        uint32_t p = spf[x];
        uint32_t e = 0;
        while (x % p == 0) { x /= p; ++e; }
        primesJ[cntJ] = p;
        expsJ[cntJ]   = e;
        ++cntJ;
    }

    /* merge prime lists and multiply contributions */
    seq_t result = 1;
    size_t ia = 0, ib = 0;
    while (ia < cntI || ib < cntJ) {
        uint32_t p;
        uint32_t expI = 0, expJ = 0;
        if (ia < cntI && (ib == cntJ || primesI[ia] < primesJ[ib])) {
            p    = primesI[ia];
            expI = expsI[ia];
            expJ = 0;
            ++ia;
        } else if (ib < cntJ && (ia == cntI || primesJ[ib] < primesI[ia])) {
            p    = primesJ[ib];
            expI = 0;
            expJ = expsJ[ib];
            ++ib;
        } else { /* equal primes */
            p    = primesI[ia];
            expI = expsI[ia];
            expJ = expsJ[ib];
            ++ia; ++ib;
        }

        seq_t term = prime_contrib(p, expI, expJ);
        result *= term;
    }
    return result;
}

/* ---------- Public sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* undefined for index 0 */

    /* Locate diagonal d such that d*(d+1)/2 >= n */
    idx_t d = 0;
    {
        idx_t disc = 1 + 8 * n;
        idx_t s    = isqrt_u64(disc);
        d = (s - 1) / 2;
        if (d * (d + 1) / 2 < n) ++d;
    }

    idx_t prev_tri = (d - 1) * d / 2;
    idx_t offset   = n - prev_tri;             /* 1‑based offset on this diagonal */

    idx_t i = offset;
    idx_t j = d + 1 - offset;

    /* Ensure SPF table covers the range needed */
    idx_t mx = i > j ? i : j;
    if (mx > 1) build_spf(mx);

    return T_value(i, j);
}
