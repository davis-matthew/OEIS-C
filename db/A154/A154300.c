/*
 * A154300  Primes of the form (1+2+...+m)/57 = A000217(m)/57.
 * https://oeis.org/A154300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: full, pass, 4 terms through n = 4 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A154300: primes of the form T(m)/57, where T(m)=m(m+1)/2
   Non‑nonn, finite; only m = 18, 38, 57, 113 give primes.
   The function computes the list once and returns the n‑th term.
   Out of bounds indices return 0.
*/
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
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
    /* static state: the list of primes and its length */
    static seq_t primes[4];
    static size_t count = 0;
    static bool ready   = false;

    if (!ready)
    {
        const idx_t LIM = 2 * 57;          /* 114 */
        for (idx_t m = 1; m <= LIM; ++m)
        {
            /* compute T(m) in an intermediate unsigned 64‑bit type */
            uint64_t mm = (uint64_t)m;
            uint64_t t  = mm * (mm + 1) / 2;
            if (t % 57 != 0)
                continue;
            uint64_t p = t / 57;
            if (is_prime_big((seq_t)p))
                primes[count++] = (seq_t)p;
        }
        ready = true;
    }

    if (n == 0 || n > count)
        return 0;      /* no term for this index */
    return primes[n - 1];
}
