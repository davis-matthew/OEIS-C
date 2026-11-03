/*
 * A182231  Primes that are palindromic in base 32.
 * https://oeis.org/A182231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th prime that is a palindrome in base 32
 * Range:     u8 n <= 11, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
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

static seq_t build_pal_ull(uint64_t half, bool odd)
{
    uint64_t res   = half;
    uint64_t temp  = odd ? half / 32 : half;
    uint64_t rev   = 0;

    while (temp > 0) {
        rev = rev * 32 + (temp % 32);
        temp /= 32;
    }

    while (rev > 0) {
        res = res * 32 + (rev % 32);
        rev /= 32;
    }

    if (sizeof(seq_t) <= 8) {
        seq_t casted = (seq_t)res;
        if ((uint64_t)casted != res) return 0;   /* overflow */
        return casted;
    }
    return (seq_t)res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for 0 */
    idx_t count = 0;
    idx_t L = 1;

    while (1) {
        int half_len = (int)((L + 1) / 2);
        uint64_t pow32 = 1;
        for (int i = 1; i < half_len; ++i) pow32 *= 32ULL;
        uint64_t start = pow32;
        uint64_t end   = pow32 * 32ULL - 1;
        if (end < start) break;    /* overflow, cannot generate further */

        for (uint64_t h = start; h <= end; ++h) {
            seq_t pal = build_pal_ull(h, (L % 2 == 1));
            if (pal == 0) continue;
            if (is_prime_big(pal)) {
                ++count;
                if (count == n) return pal;
            }
        }
        ++L;
    }
    return 0;  /* should not reach for valid n */
}
