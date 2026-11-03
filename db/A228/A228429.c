/*
 * A228429  Number of ways to write n = x + y (x, y > 0) with p(39, x) + p(40,
 *          y) prime, where p(m, k) denotes the m-gonal number (m-2)*k*(k-1)/2
 *          + k.
 * https://oeis.org/A228429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 60, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9544 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of ordered pairs (x, y) with x > 0, y > 0,
 *                x + y = n, such that p(39, x) + p(40, y) is prime.
 *   p(m, k) = (m-2)·k·(k-1)/2 + k
 * This implementation counts all such pairs by brute force.
 * It uses the generic prime test `is_prime_big` from the utils set.
 */

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

/* Compute the m‑gonal number p(m, k)  (m ≥ 3, k ≥ 1) */
static inline seq_t polygon(int m, idx_t k)
{
    seq_t kk = (seq_t)k;
    seq_t tmp = kk * (kk - 1) / 2;          /* k(k-1)/2 */
    return (seq_t)(m - 2) * tmp + kk;       /* (m-2)*tmp + k */
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    seq_t cnt = 0;
    for (idx_t x = 1; x < n; ++x)
    {
        idx_t y = n - x;
        seq_t px = polygon(39, x);
        seq_t py = polygon(40, y);
        seq_t sum = px + py;
        if (is_prime_big(sum))
            ++cnt;
    }
    return cnt;
}
