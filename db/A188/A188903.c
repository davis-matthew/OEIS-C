/*
 * A188903  a(n) is the least power of 2 such that 2n+1 - a(n) is prime, or 0
 *          if no such prime exists.
 * https://oeis.org/A188903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 126, u16 n <= 126, u32 n <= 126, u64 n <= 126,
 *            u128 n <= 126, big any n
 * Validated: partial, pass, 127 terms through n = 126 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = the smallest power of 2 such that 2*n+1 - a(n) is prime
 *  (or 0 if no such prime exists).  */
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
    /* 2*n+1 as a seq_t value. */
    seq_t target = ((seq_t)n * (seq_t)2) + (seq_t)1;

    /* Enumerate powers of two, starting with 1. */
    seq_t p = (seq_t)1;
    while (p != (seq_t)0 && p <= target)
    {
        /* If 2*n+1 - p is prime, we found the minimal power of two. */
        seq_t diff = target - p;
        if (is_prime_big(diff))
            return p;

        /* Stop if the next power would exceed the target or overflow. */
        if (p > target / (seq_t)2)
            break;

        p <<= 1;   /* Go to the next power of two. */
    }

    /* No suitable power of two found. */
    return (seq_t)0;
}
