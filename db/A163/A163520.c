/*
 * A163520  a(n) is the least integer x such that n < x and the product n*x is
 *          divisible by an integer y where n < y < x.
 * https://oeis.org/A163520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least x>n such that there exists y with n<y<x and y|n*x
 * Range:     u8 n <= 25, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*----------------------------------------------------------------------------*/
/*-------------  Utility Functions  -------------------------------------------*/
/*----------------------------------------------------------------------------*/
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* ---- Integer square root (floor) ---------------------------------------- */
static idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n) lo = mid + 1;
        else          hi = mid - 1;
    }
    return hi;
}

/* ---- Smallest prime factor table ---------------------------------------- */
#define SPF_LIMIT   40000
static idx_t spf[SPF_LIMIT + 1];
static bool spf_built = false;

static void build_spf(void)
{
    for (idx_t i = 0; i <= SPF_LIMIT; ++i) spf[i] = i;
    spf[0] = 0; if (SPF_LIMIT >= 1) spf[1] = 1;
    for (idx_t i = 2; i * i <= SPF_LIMIT; ++i) {
        if (spf[i] == i) {
            for (idx_t j = i * i; j <= SPF_LIMIT; j += i)
                if (spf[j] == j) spf[j] = i;
        }
    }
    spf_built = true;
}

/* ---- Factorisation ------------------------------------------------------- */
static void factorise(idx_t n, int primes[], int exps[], int *cnt)
{
    *cnt = 0;
    while (n > 1) {
        idx_t p = spf[(idx_t)n];
        int e = 0;
        while (n % p == 0) {
            n /= p;
            ++e;
        }
        primes[(*cnt)] = (int)p;
        exps[(*cnt)]   = e;
        ++(*cnt);
    }
}

/* ---- Divisor generation ------------------------------------------------- */
static void gen_divisors(const int primes[], const int exps[],
                         int total, int idx, seq_t cur,
                         const idx_t n, const idx_t x, bool *found)
{
    if (*found) return;
    if (cur > n && cur < x) {
        *found = true;
        return;
    }
    if (idx == total) return;

    seq_t val = cur;
    for (int e = 0; e <= exps[idx]; ++e) {
        if (*found) return;
        gen_divisors(primes, exps, total, idx + 1, val, n, x, found);
        val *= (seq_t)primes[idx];
        if (val > x) break;
    }
}

/*----------------------------------------------------------------------------*/
/*----------------------- Main Sequence Function ------------------------------*/
/*----------------------------------------------------------------------------*/

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1; 0 is unused */

    /* initialise smallest prime factor table once */
    if (!spf_built) build_spf();

    /* prime shortcut */
    if (n > 1 && spf[n] == n) {
        return (seq_t)2 * (seq_t)(n + 1);
    }

    /* perfect square shortcut: a(m^2) = (m+1)^2 */
    idx_t s = isqrt_u64(n);
    if (s * s == n) {
        return (seq_t)(s + 1) * (seq_t)(s + 1);
    }

    /* n = m(m-1) shortcut: a(m(m-1)) = m(m+1) */
    idx_t t = 1 + 4 * n;
    idx_t st = isqrt_u64(t);
    if (st * st == t) {
        idx_t m = (1 + st) / 2;
        if (m * (m - 1) == n) {
            return (seq_t)m * (seq_t)(m + 1);
        }
    }

    /* generic search ------------------------------------------------------- */
    int primesN[20], expsN[20], cntN;
    factorise(n, primesN, expsN, &cntN);

    /* search x from n+1 up to 2n+2 (always finite) */
    for (idx_t x = n + 1; x <= n + 2 * n + 2; ++x) {
        int primesX[20], expsX[20], cntX;
        factorise(x, primesX, expsX, &cntX);

        /* merge factorizations into product */
        int primesAll[20], expsAll[20], cntAll = 0;
        for (int i = 0; i < cntN; ++i) {
            primesAll[cntAll] = primesN[i];
            expsAll[cntAll]   = expsN[i];
            ++cntAll;
        }
        for (int i = 0; i < cntX; ++i) {
            int p = primesX[i];
            bool found = false;
            for (int j = 0; j < cntAll; ++j) {
                if (primesAll[j] == p) {
                    expsAll[j] += expsX[i];
                    found = true;
                    break;
                }
            }
            if (!found) {
                primesAll[cntAll] = p;
                expsAll[cntAll]   = expsX[i];
                ++cntAll;
            }
        }

        bool haveDiv = false;
        gen_divisors(primesAll, expsAll, cntAll, 0, (seq_t)1, n, x, &haveDiv);
        if (haveDiv) return (seq_t)x;
    }

    /* fallback (should never be reached) */
    return (seq_t)0;
}
