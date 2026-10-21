/*
 * A076763  1-apexes of omega: numbers n such that omega(n-1) < omega(n) >
 *          omega(n+1), where omega(m) = the number of distinct prime factors
 *          of m.
 * https://oeis.org/A076763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computation of A076763—1‑apexes of the ω function.
 *  An integer n is a 1‑apex if ω(n‑1) < ω(n) > ω(n+1),
 *  where ω(m) is the number of distinct prime factors of m.
 *
 *  The routine builds a sieve of smallest prime factors and
 *  a table of ω values as needed, then keeps a dynamically
 *  resizable list of all positions that satisfy the above
 *  inequality.  The function a(idx_t n) returns the n‑th such
 *  position, with a(1) = 6.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/*  Static tables for prime factors and ω-values  */
static uint64_t *spf = NULL;          /* smallest prime factor, 1‑based   */
static uint8_t  *omega = NULL;        /* ω(i) for 0‑based index          */
static idx_t    spf_lim = 0;          /* largest index computed in spf   */

/*  Static list of discovered 1‑apex positions  */
static seq_t   *apex_arr = NULL;      /* dynamic array of seq_t values   */
static size_t  apex_cap = 0;          /* current allocation size        */
static size_t  apex_cnt = 0;          /* number of stored 1‑apexes      */

/*  Index up to which 1‑apex test has been performed  */
static idx_t last_checked = 1;        /* start at 1 so next i = 2       */

/*  Integer square root of a 64‑bit number using 128‑bit arithmetic  */
static idx_t floor_isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = (idx_t)1ULL << ((sizeof(idx_t) * 8u) >> 1);
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if ((unsigned __int128)mid * mid <= (unsigned __int128)n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/*  Build SPF and ω tables up to at least ‘limit’.  */
static void rebuild_tables(idx_t limit)
{
    if (limit <= spf_lim) return;               /* already present */

    /*  Allocate space for new indices */
    spf   = (uint64_t *)realloc(spf,  sizeof(uint64_t) * (limit + 1));
    omega = (uint8_t  *)realloc(omega, sizeof(uint8_t)  * (limit + 1));
    if (!spf || !omega) exit(1);                /* memory failure */

    /*  Initial sieve for primes */
    for (idx_t i = spf_lim + 1; i <= limit; ++i) spf[i] = i;

    idx_t sqrt_lim = floor_isqrt_u64(limit);
    for (idx_t p = 2; p <= sqrt_lim; ++p) {
        if (spf[p] != p) continue;             /* not prime */
        uint64_t start = (uint64_t)p * (uint64_t)p;
        if (start > (uint64_t)limit) continue;
        for (idx_t j = start; j <= limit; j += p)
            if (spf[j] == j) spf[j] = p;
    }

    /*  Compute ω values in one pass  */
    omega[0] = 0;
    if (limit >= 1) omega[1] = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        uint64_t p = spf[i];
        idx_t r = i / p;
        omega[i] = (spf[r] == p) ? omega[r] : (omega[r] + 1);
    }

    spf_lim = limit;
}

/*  Store a new 1‑apex position */
static void append_apex(seq_t val)
{
    if (apex_cap == 0) {
        apex_cap = 16;
        apex_arr = (seq_t *)malloc(apex_cap * sizeof(seq_t));
        if (!apex_arr) exit(1);
    } else if (apex_cnt >= apex_cap) {
        apex_cap <<= 1;
        apex_arr = (seq_t *)realloc(apex_arr, apex_cap * sizeof(seq_t));
        if (!apex_arr) exit(1);
    }
    apex_arr[apex_cnt++] = val;
}

/*  Ensure that at least ‘n’ 1‑apex positions have been recorded.  */
static void ensure_n_apexes(idx_t n)
{
    while ((idx_t)apex_cnt < n) {
        idx_t new_limit = (spf_lim == 0) ? 64 : spf_lim * 2;
        if (new_limit <= spf_lim) new_limit = spf_lim + 64;   /* safety  */

        rebuild_tables(new_limit);

        /*  Scan new interval for 1‑apexes  */
        for (idx_t i = last_checked + 1; i + 1 <= new_limit; ++i) {
            if (omega[i - 1] < omega[i] && omega[i] > omega[i + 1]) {
                append_apex((seq_t)i);
            }
        }

        last_checked = new_limit - 1;
    }
}

/*  Public sequence function  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined but harmless */
    ensure_n_apexes(n);                    /* guarantee apex_cnt ≥ n  */
    return apex_arr[n - 1];                /* a(1) is the first element */
}
