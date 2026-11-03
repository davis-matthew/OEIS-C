/*
 * A057217  a(n) = smallest positive integer k such that 1 + n*k! is a prime.
 * https://oeis.org/A057217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{k | 1 + n*k! is prime}. Implements a search with a
 *            128‑bit intermediate to avoid overflow on 64‑bit builds. When
 *            128‑bit overflow is detected it switches to a full
 *            arbitrary‑precision search using seq_t
 * Range:     u8 n <= 18, u16 n <= 58, u32 n <= 78, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 119 terms through n = 9756 (2026-11-03)
 * Helpers:   is_prime_seq, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_seq.h"
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

/* Fallback search that works for all seq_t types (arbitrary precision on
   builds where seq_t is bigger than 64 bits).  It continues the search
   from the given k value. */
static seq_t big_search(seq_t nn, idx_t start_k)
{
    seq_t fact = 1;
    idx_t k = 1;

    /* build factorial up to (start_k-1)! */
    while (k < start_k) {
        fact *= (seq_t)k;
        ++k;
    }
    /* now multiply by start_k to get k! */
    fact *= (seq_t)k;

    for (;;) {
        seq_t candidate = 1 + nn * fact;
        if (is_prime_big(candidate))
            return (seq_t)k;
        ++k;
        fact *= (seq_t)k;
    }
}

seq_t a(idx_t n)
{
    /* calculate using unsigned __int128 to avoid overflow on 64‑bit builds */
    __int128 fact = 1;
    idx_t k = 1;

    for (;;) {
        fact *= k;                            /* fact = k!  */
        __int128 prod = (__int128)n * fact;   /* n*k!    */

        if (prod > UINT64_MAX)                /* 64‑bit overflow detected   */
            return big_search((seq_t)n, k);   /* switch to arbitrary‑precision path */

        unsigned long long candidate = (unsigned long long)(prod + 1);
        if (is_prime_seq(candidate))
            return (seq_t)k;

        ++k;
    }
}
