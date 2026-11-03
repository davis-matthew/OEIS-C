/*
 * A118132  Triangle read by rows n>=0: the largest prime <= m^n+2 in columns
 *          m=3..n+3.
 * https://oeis.org/A118132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = largest prime ≤ (m^r + 2), where r = floor( (-1 + sqrt(8n
 *            + 1)) / 2 ) (row number) k = n - r(r + 1)/2 (index within row) m
 *            = 3 + k (column base)
 * Range:     u8 n <= 10, u16 n <= 24, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 39 (2026-11-03)
 * Helpers:   ipow, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"
#include "../utils/is_prime_u64.h"

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

static seq_t prev_prime(seq_t x)
{
    seq_t cand = x;
    for (;;)
    {
        if (is_prime_big(cand))
            return cand;
        cand--;
    }
}

seq_t a(idx_t n)
{
    /* Determine the row r of index n */
    idx_t r = (idx_t)((sqrt(8.0 * n + 1.0) - 1.0) / 2.0);

    /* Compute the start index of this row */
    idx_t start = r * (r + 1) / 2;

    /* Position k within the row */
    idx_t k = n - start;

    /* Column base m */
    idx_t m = 3 + k;

    /* Compute m^r + 2 */
    seq_t val = ipow((seq_t)m, (seq_t)r) + (seq_t)2;

    /* Find the largest prime ≤ val */
    return prev_prime(val);
}
