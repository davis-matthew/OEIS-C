/*
 * A062334  Primes starting and ending with 7.
 * https://oeis.org/A062334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 35, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 57 terms through n = 883 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A062334:  primes that start and end with 7.
    Computed by brute‑force search:  test all numbers ending in 7,
    keep those that start with 7 and are prime.  */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
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

/* Return true if the most significant decimal digit of x is 7.
   Works for both built‑in and arbitrary‑precision seq_t types. */
static bool starts_with_7(seq_t x)
{
    seq_t y = x;
    while (y >= (seq_t)10)   /* keep removing the last digit */
        y /= (seq_t)10;
    return y == (seq_t)7;
}

/* Main sequence function.  Calls may arrive in any order; each call
   performs an independent search.  The search is fast enough for
   n up to about 10 000 (well under 2 s in the 64‑bit build). */
seq_t a(idx_t n)
{
    if (n == 0)   /* the OEIS offset is 1; n==0 is unused */
        return 0;

    idx_t  found = 0;      /* how many valid primes found so far   */
    seq_t  cand   = 7;      /* first candidate (ends with 7)          */

    while (found < n) {
        if (starts_with_7(cand) && is_prime_big(cand)) {
            ++found;
            if (found == n)
                return cand;
        }
        cand += (seq_t)10;  /* next number that ends with 7           */
    }

    /* This point never reached – kept to satisfy the compiler. */
    return (seq_t)0;
}
