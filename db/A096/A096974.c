/*
 * A096974  Numbers n such that A096123(n) = n!/(nextprime(n/2)-1)!.
 * https://oeis.org/A096974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with A096123(k) = k!/(q-1)!, q = least prime >=
 *            k/2; computed by scan using t(k)=min over primes p of max t with
 *            2*v_p(t!) <= v_p(k!)
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static idx_t *A096974_terms = 0;
static size_t A096974_cnt = 0, A096974_cap = 0;
static idx_t A096974_scanned = 0;
static bool *A096974_isp = 0;
static idx_t A096974_lim = 0;

static idx_t A096974_vpf(idx_t m, idx_t p)
{
    idx_t e = 0;
    while (m) { m /= p; e += m; }
    return e;
}

static void A096974_sieve(idx_t lim)
{
    free(A096974_isp);
    A096974_isp = (bool *)calloc((size_t)(lim + 1), sizeof(bool));
    A096974_lim = lim;
    for (idx_t i = 2; i <= lim; ++i) A096974_isp[i] = true;
    for (idx_t i = 2; i * i <= lim; ++i)
        if (A096974_isp[i])
            for (idx_t j = i * i; j <= lim; j += i) A096974_isp[j] = false;
}

static bool A096974_check(idx_t m)
{
    if (m == 1) return true;
    idx_t q = (m + 1) / 2;
    if (q < 2) q = 2;
    while (!A096974_isp[q]) ++q;
    idx_t T = q - 1;
    bool exists = (T == m - 1);
    for (idx_t p = 2; p <= m; ++p)
    {
        if (!A096974_isp[p]) continue;
        idx_t vn = A096974_vpf(m, p);
        idx_t vT = A096974_vpf(T, p);
        if (2 * vT > vn) return false;
        if (!exists)
        {
            idx_t vT1 = A096974_vpf(T + 1, p);
            if (2 * vT1 > vn) exists = true;
        }
    }
    return exists;
}

seq_t a(idx_t n)
{
    while (A096974_cnt < (size_t)n)
    {
        idx_t m = A096974_scanned + 1;
        if (m + 2 > A096974_lim) A096974_sieve(2 * m + 1024);
        if (A096974_check(m))
        {
            if (A096974_cnt == A096974_cap)
            {
                A096974_cap = A096974_cap ? A096974_cap * 2 : 256;
                A096974_terms = (idx_t *)realloc(A096974_terms, A096974_cap * sizeof(idx_t));
            }
            A096974_terms[A096974_cnt++] = m;
        }
        A096974_scanned = m;
    }
    return (seq_t)A096974_terms[n - 1];
}
