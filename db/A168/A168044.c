/*
 * A168044  Half of the even nonisolated nonprimes A167692.
 * https://oeis.org/A168044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = index of numbers n such that at least one of 2n-1 and
 *            2n+1 is composite (with the first term a(1) = 0)
 * Range:     u8 n <= 122, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 87 terms through n = 940 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
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

seq_t a(idx_t n)
{
    /* The first term is always 0. */
    if (n == 1)
        return (seq_t)0;

    /* Count the terms that have been found so far.
       Initially we have already counted 0 as the first term. */
    idx_t count = 1;
    /* Candidate value for n.  We start checking from 1. */
    idx_t candidate = 1;

    for (;;)
    {
        /* Compute 2*n-1 and 2*n+1. */
        seq_t p1 = (seq_t)candidate * 2 - 1; /* 2n-1 */
        seq_t p2 = p1 + 2;                   /* 2n+1 */

        /* A number is not composite if it is <=1 or it is prime. */
        bool p1_not_composite = (p1 <= (seq_t)1) || is_prime_big(p1);
        bool p2_not_composite = (p2 <= (seq_t)1) || is_prime_big(p2);

        /* Include candidate if at least one of p1 or p2 is composite. */
        if (!(p1_not_composite && p2_not_composite))
        {
            count++;
            if (count == n)
                return (seq_t)candidate;
        }

        candidate++;
    }
}
