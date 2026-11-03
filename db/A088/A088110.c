/*
 * A088110  Numbers n such that 2^n + n and 2^n - n are noncomposite.
 * https://oeis.org/A088110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  more, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: partial, pass, 4 terms through n = 4 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n‑th integer m (starting with m = 0) such that 
   both 2^m + m and 2^m – m are either prime or equal to 1. */

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
    /* OEIS offset: a(1) corresponds to m = 0 */
    if (n == 0) {
        return (seq_t)0;      /* undefined for n = 0, return 0 safely */
    }

    idx_t  count = 0;          /* how many qualifying m's have been found */
    idx_t  m     = 0;          /* current exponent being tested */
    seq_t  pow2  = (seq_t)1;   /* 2^m */

    while (1) {
        seq_t sum = pow2 + (seq_t)m;
        seq_t diff = pow2 - (seq_t)m;

        /* A number is considered “non‑composite” if it is 1 or prime. */
        bool ok = ((diff == (seq_t)1) || is_prime_big(diff)) &&
                  ((sum  == (seq_t)1) || is_prime_big(sum));

        if (ok) {
            ++count;
            if (count == n) {
                return (seq_t)m;   /* return the n‑th qualifying m */
            }
        }

        /* Prepare for the next exponent.  Detect overflow only for
           unsigned integer types (sizeof(seq_t) ≤ 8).  For arbitrary
           precision types we assume no overflow occurs. */
        ++m;
        if (sizeof(seq_t) <= 8) {
            /* (~(seq_t)0) is the type's maximum value.  If pow2 is larger
               than half that value, multiplying by 2 would overflow. */
            if (pow2 > ((~(seq_t)0) >> 1)) {
                break;          /* cannot test larger exponents safely */
            }
        }
        pow2 = pow2 * (seq_t)2;        /* advance to 2^(m+1) */
    }

    /* If the loop terminates without finding the n‑th term, the
       sequence is exhausted for the current build.  Return 0 as a
       sentinel. */
    return (seq_t)0;
}
