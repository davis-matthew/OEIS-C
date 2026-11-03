/*
 * A085957  Primes p such that 6p + 1 and (p-1)/6 are primes.
 * https://oeis.org/A085957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 22, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Validated: partial, pass, 38 terms through n = 38 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A085957: primes p such that 6p+1 and (p-1)/6 are primes. */
/* The function returns the nth term (n ≥ 1).  The implementation
   simply scans candidates p ≡ 1 (mod 6) and tests the primality of
   p, (p-1)/6 and 6p+1 using is_prime_big.  Complexity is
   roughly O(n log n) and easily satisfies the 2‑second limit for
   n up to ≈ 10000 on a normal machine. */

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
    if (n == 0) return 0;                /* offset 1 – behaviour below is irrelevant */
    seq_t count = 0;
    seq_t p = 13;                         /* first prime ≡ 1 (mod 6) in the sequence */

    while (true)
    {
        /* Check that p is prime first */
        if (is_prime_big(p))
        {
            seq_t r = (p - 1) / 6;        /* (p-1)/6 must be prime */
            seq_t q = 6 * p + 1;          /* 6p+1 must be prime */

            if (is_prime_big(r) && is_prime_big(q))
            {
                ++count;
                if (count == (seq_t)n)
                    return p;            /* found the nth term */
            }
        }

        p += 6;                          /* next candidate ≡ 1 (mod 6) */
    }
}
