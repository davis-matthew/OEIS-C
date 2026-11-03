/*
 * A154303  Primes of the form (1+2+...+m)/90 = A000217(m)/90.
 * https://oeis.org/A154303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy, bref
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: full, pass, 4 terms through n = 4 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A154303: Primes of the form (1+2+...+m)/90 = m(m+1)/(2·90) = m(m+1)/180.
    The sequence is finite: the only m ≤ 180 giving an integer prime are 35, 44, 179, 180.
    The implementation searches for m up to 200; this is sufficient for every
    supported seq_t (uint8_t, uint16_t, uint32_t, uint64_t, or arbitrary precision)
    because m(m+1)/180 ≤ 223 for m ≤ 200, which fits in a uint8_t. */

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
    if (n == 0)          /* sequence offset is 1, return 0 for n==0 */
        return 0;
    idx_t count   = 0;
    const idx_t MAX_M = 200;   /* upper limit safe for all seq_t sizes */
    for (idx_t m = 1; m <= MAX_M; ++m) {
        idx_t prod = m * (m + 1);              /* m·(m+1)  (fits in uint64_t) */
        if ((prod % 180u) != 0u)               /* must be divisible by 180 */
            continue;
        idx_t val64 = prod / 180u;             /* candidate prime (fits in idx_t) */
        seq_t val = (seq_t)val64;              /* cast to seq_t                */
        if ((seq_t)val64 != val)              /* overflow check for small seq_t */
            continue;
        if (is_prime_big(val)) {              /* primality test */
            ++count;
            if (count == n)                   /* n‑th prime found */
                return val;
        }
    }
    return 0;                                 /* no more terms beyond the known ones */
}
