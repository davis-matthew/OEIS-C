/*
 * A218391  Let k be the n-th odd composite, then a(n) is the smallest w*x
 *          such that w + x = (k-1)/2, y + z = (k+1)/2, and w*x = y*z.
 * https://oeis.org/A218391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 343, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 9909 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: brute‑force search for each odd composite k.
   For k, find the smallest product w*x with w+x=(k-1)/2,
   y+z=(k+1)/2, and w*x=y*z.  The algorithm checks all w, verifies
   that the product can be written as y*(s2-y) by solving the quadratic
   y^2-(k+1)/2*y+p=0 and selecting the minimum product. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static seq_t *odd_composites = NULL;   /* stored odd composites */
static size_t comps_len = 0;           /* number of composites stored */
static size_t comps_cap = 0;           /* capacity of the array */
static seq_t next_composite = 9;       /* next odd number to test */

/* Grow the composite array to hold at least 'need' elements. */
static void ensure_comps_capacity(size_t need)
{
    if (need <= comps_cap) return;
    size_t new_cap = comps_cap ? comps_cap * 2 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *new_ptr = mem_resize(seq_t, odd_composites, new_cap);
    if (!new_ptr) abort();             /* never expected in this test */
    odd_composites = new_ptr;
    comps_cap = new_cap;
}

/* Generate enough odd composites to reach the n-th one. */
static seq_t nth_odd_composite(idx_t n)
{
    while (comps_len < n) {
        seq_t cand = next_composite;
        next_composite += (seq_t)2;   /* next odd number */

        /* A candidate is a composite if it is odd, >1, and not prime. */
        if ((cand & 1) && cand > 1 && !is_prime_big(cand)) {
            ensure_comps_capacity(comps_len + 1);
            odd_composites[comps_len++] = cand;
        }
    }
    return odd_composites[n - 1];
}

seq_t a(idx_t n)
{
    seq_t k = nth_odd_composite(n);
    seq_t S = (k - 1) / 2;          /* sum for w + x */
    seq_t Splus = S + 1;            /* sum for y + z */

    seq_t best = 0;
    bool found = false;
    idx_t Sidx = (idx_t)S;

    for (idx_t w = 1; w < Sidx; ++w) {
        seq_t w_seq = (seq_t)w;
        seq_t prod = w_seq * (S - w_seq);

        seq_t four_prod = (seq_t)4 * prod;
        seq_t sp_sq = Splus * Splus;
        if (sp_sq < four_prod) continue;   /* discriminant negative */

        seq_t D = sp_sq - four_prod;
        seq_t sqrtD = floor_sqrt(D);
        if (sqrtD * sqrtD != D) continue;  /* not a perfect square */

        /* Check parity for integer y. */
        if (((Splus + sqrtD) & 1) != 0) continue;
        seq_t y = (Splus + sqrtD) / 2;
        if (y < 1 || y > S) continue;      /* out of allowed range */

        if (!found || prod < best) {
            best = prod;
            found = true;
        }
    }
    return best;   /* guaranteed to be found for all n>=1 */
}
