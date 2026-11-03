/*
 * A078768  a(n) is the number of iterations of k -> sigma(k) + 1 needed to
 *          reach a prime, or 0 if no prime is reached.
 * https://oeis.org/A078768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-03)
 * Helpers:   is_pow2, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A078768
   a(n) = number of iterations of k -> sigma(k)+1 needed to reach a prime,
   or 0 if no prime is reached.  */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_pow2.h"
#include "../utils/is_prime_u64.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

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
    /* If the start is a power of two greater than 2 the sequence never
       reaches a prime because sigma(2^k)+1 = 2^(k+1). */
    if (is_pow2((seq_t)n) && n > 2)
        return (seq_t)0;

    seq_t cur   = (seq_t)n;
    seq_t steps = 0;

    for (;;)
    {
        /* cur must fit in idx_t to call sigma_of */
        if (cur > (idx_t)-1)
            return (seq_t)0;

        idx_t idx = (idx_t)cur;
        seq_t next = sigma_of(idx) + 1;
        ++steps;

        if (is_prime_big(next))
            return steps;

        if (is_pow2(next) && next > 2)
            return (seq_t)0;

        cur = next;

        /* safety watchdog – the known sequences terminate far before this
           limit for all n ≤ 10000. */
        if (steps > 5000)
            return (seq_t)0;
    }
}
