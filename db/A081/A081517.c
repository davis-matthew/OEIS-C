/*
 * A081517  Consider the smallest number m which can be expressed as the sum
 *          of n distinct numbers coprime to m. Sequence gives triangle (read
 *          by rows) of the set of coprime numbers pertaining to m. When there
 *          is a choice, use the lexicographically earliest solution.
 * https://oeis.org/A081517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(1) = 1 For n ≥ 2 let r be the smallest integer with r·(r+1)/2
 *            ≥ n. If the position in that row is ≤ r−1, then a(n) = that
 *            position. Otherwise a(n) = p − (r−1)·r/2 where p is the least
 *            prime ≥ r·(r+1)/2. This implements the OEIS triangle read by
 *            rows
 * Range:     u8 n <= 1769, u16 n <= 7260, u32 n <= 7260, u64 n <= 7260,
 *            u128 n <= 7260, big any n
 * Validated: standard, pass, 107 terms through n = 6627 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
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

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;

    /* Find row r: smallest r such that r*(r+1)/2 ≥ n. */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t t = (__uint128_t)mid * (mid + 1) / 2;
        if (t >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Sum of elements in all previous rows: (r-1)*r/2. */
    idx_t prev_sum = (idx_t)((__uint128_t)(r - 1) * r / 2);
    idx_t col = n - prev_sum;  /* 1‑based column index within row r. */

    if (col <= r - 1)
        return (seq_t)col;  /* lexicographically earliest choice. */

    /* Compute the least prime ≥ r*(r+1)/2. */
    seq_t target = (seq_t)r * ((seq_t)r + 1) / 2;
    seq_t m = target;
    while (!is_prime_big(m))
        ++m;

    return (m - (seq_t)prev_sum);
}
