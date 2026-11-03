/*
 * A140330  Smallest nonprime with Hamming weight n (i.e., with exactly n 1's
 *          when written in binary).
 * https://oeis.org/A140330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   a(n) = MIN{ k | k has Hamming weight n and k is nonprime }
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Validated: partial, pass, 31 terms through n = 31 (2026-11-03)
 * Helpers:   ipow_idx, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: iterate over numbers with n set bits starting from the
   Mersenne number 2^n – 1, testing each for primality with Gosper's hack
   to generate the next integer with the same number of 1 bits. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"
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
    /* For n == 0 the sequence is undefined; return 0 safely. */
    if (n == (idx_t)0) return (seq_t)0;

    /* Start from the smallest n‑bit number consisting entirely of 1's. */
    seq_t cand = ipow_idx((seq_t)2, n) - (seq_t)1;

    /* Find the first non‑prime among numbers with popcount n. */
    while (is_prime_big(cand)) {
        /* Gosper's hack: next integer with the same popcount. */
        seq_t smallest = cand & -cand;          /* lowest set bit      */
        seq_t ripple   = cand + smallest;       /* add lowest set bit  */
        seq_t ones     = cand ^ ripple;         /* bits that flipped   */
        ones = (ones >> 2) / smallest;          /* rearrange higher bits */
        cand = ripple | ones;                   /* new candidate       */
    }

    return cand;
}
