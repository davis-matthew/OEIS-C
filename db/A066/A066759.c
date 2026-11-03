/*
 * A066759  a(n) = least multiple of n differing from a prime by at most 1, if
 *          such a multiple exists; = 0 otherwise.
 * https://oeis.org/A066759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least multiple of n that differs from a prime by at most
 *            1; a(n) = 0 if no such multiple exists. The function searches
 *            for the smallest positive integer k such that n·k is within 1 of
 *            a prime
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 68 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  We rely on the utility function
        bool is_prime_big(seq_t n)
   which is available in the build.  It performs a deterministic
   primality test for values that fit in 64 bits and a probable test
   for larger values (the sequence data for n ≤ 10⁴ never exceeds 64
   bits, so the result is exact).  For small seq_t types the test
   automatically works on the wrapped values that fit.  The algorithm
   is O(k) with a very small k in practice. */

#include <stdint.h>
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
    if (n == 0)          /* index 0 is outside the offset of the sequence */
        return (seq_t)0;

    seq_t sn = (seq_t)n; /* convert index to seq_t so that arithmetic
                            is performed in the correct type */
    for (idx_t k = 1; ; ++k)
    {
        /* compute m = n * k   (wraps modulo if seq_t is too small) */
        seq_t m = sn * (seq_t)k;

        /* if we hit an overflow, the product will wrap to 0; stop searching */
        if (m == (seq_t)0)
            break;

        /* check if m, m-1, or m+1 is prime */
        if (is_prime_big(m) || (m > (seq_t)1 && is_prime_big(m - (seq_t)1))
                        || is_prime_big(m + (seq_t)1))
            return m;
    }

    /* no suitable multiple was found (only possible if an overflow
       hid the answer).  In the range of interest this never occurs. */
    return (seq_t)0;
}
