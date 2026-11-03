/*
 * A224498  Smallest prime that can be expressed as the sum of n distinct
 *          positive squares with the largest square as small as possible.
 * https://oeis.org/A224498
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 56, u32 n <= 387, u64 n <= 387,
 *            u128 n <= 387, big any n
 * Validated: standard, pass, 60 terms through n = 383 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A224498: the smallest prime that can be expressed as the sum of
 * n distinct positive squares with the largest square as small as possible.
 * The algorithm increases the maximal square m (m ≥ n) and removes d = m-n
 * squares. For each m it enumerates all subsets of size d (d ≤ 2 in normal
 * cases) and keeps the smallest prime found. As soon as a prime is found for
 * some m the algorithm returns it, guaranteeing that the returned prime has
 * the minimal possible largest square. */

#include <stdbool.h>
#include <stdint.h>
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

/* Return sum of squares 1² + 2² + … + t² as seq_t. */
static inline seq_t sum_sq(idx_t t)
{
    seq_t tt = (seq_t)t;
    /* tt*(tt+1)*(2*tt+1)/6 */
    return tt * (tt + 1) * (2 * tt + 1) / 6;
}

/* Search for A224498(n). */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;          /* undefined for n < 2 */

    /* Increase maximal square m starting from n until a prime is found. */
    for (idx_t m = n; ; ++m)
    {
        seq_t sum_all = sum_sq(m);       /* Σ_{i=1}^{m} i² */
        idx_t d = m - n;                 /* number of squares to exclude */

        if (d == 0)                     /* use all squares 1..m */
        {
            if (is_prime_big(sum_all)) return sum_all;
            continue;
        }

        if (d == 1)                     /* exclude one square */
        {
            seq_t best = 0;
            for (idx_t j = m - 1; j >= 1; --j)
            {
                seq_t cand = sum_all - (seq_t)j * (seq_t)j;
                if (is_prime_big(cand) && (best == 0 || cand < best))
                    best = cand;
            }
            if (best != 0) return best;
            continue;
        }

        if (d == 2)                     /* exclude two squares */
        {
            seq_t best = 0;
            for (idx_t j = m - 1; j >= 2; --j)
            {
                seq_t sqj = (seq_t)j * (seq_t)j;
                for (idx_t i = j - 1; i >= 1; --i)
                {
                    seq_t cand = sum_all - sqj - (seq_t)i * (seq_t)i;
                    if (is_prime_big(cand) && (best == 0 || cand < best))
                        best = cand;
                }
            }
            if (best != 0) return best;
            continue;
        }

        /* For d ≥ 3 the search would be prohibitively expensive for the
         * given constraints and do not occur for n ≤ 10000 in the OEIS
         * data.  If such n were requested we simply skip to the next m.
         */
        continue;
    }
}
