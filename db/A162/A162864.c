/*
 * A162864  Integers n, divisible by six, for which t = (4^n - 5 * 2^n - 4 *
 *          4^(n^2 - 2n) + 8 * 2^(n^2 - 2n)) / (4 * (n^3 - n)) is an integer.
 * https://oeis.org/A162864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A162864 – n divisible by 6 for which
   (4^n – 5·2^n – 4·4^(n^2–2n) + 8·2^(n^2–2n)) / (4·(n^3–n))
   is an integer.  The function returns the nth such integer
   (a(1) = 6).  Search is performed by testing successive
   multiples of 6; each test uses modular arithmetic with
   64‑bit types.  This implementation works for both the
   fixed‑size integer builds and the arbitrary‑precision build. */

#include <stdint.h>
#include <stdbool.h>

/* modular exponentiation for 64‑bit base, 64‑bit exponent, 64‑bit modulus */
static uint64_t mod_pow_u64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;

    while (exp != 0) {
        if (exp & 1ULL)
            result = (unsigned __int128)result * b % mod;
        b = (unsigned __int128)b * b % mod;
        exp >>= 1;
    }
    return result;
}

/* test whether n (a multiple of 6) satisfies the divisibility condition */
static bool is_valid(uint64_t n)
{
    /* denominator = 4*(n^3 – n) */
    uint64_t denom =
        4ULL * ((unsigned __int128)n * n * n - n); /* safe up to n≈10^6 */

    /* 4^n mod denom */
    uint64_t p1 = mod_pow_u64(4ULL, n, denom);
    /* 2^n mod denom */
    uint64_t p2 = mod_pow_u64(2ULL, n, denom);
    /* exponent e = n^2 – 2n */
    uint64_t e = (unsigned __int128)n * (n - 2ULL);
    /* 4^e mod denom */
    uint64_t p3 = mod_pow_u64(4ULL, e, denom);
    /* 2^e mod denom */
    uint64_t p4 = mod_pow_u64(2ULL, e, denom);

    /* numerator modulo denom:
       p1 – 5·p2 – 4·p3 + 8·p4 (mod denom) */
    uint64_t val = p1;

    uint64_t t2 = (uint64_t)((unsigned __int128)5ULL * p2 % denom);
    val += denom - t2;
    val %= denom;

    uint64_t t3 = (uint64_t)((unsigned __int128)4ULL * p3 % denom);
    val += denom - t3;
    val %= denom;

    uint64_t t4 = (uint64_t)((unsigned __int128)8ULL * p4 % denom);
    val += t4;
    val %= denom;

    return val == 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined, return 0 for safety */

    idx_t count = 0;
    uint64_t cand = 6ULL;              /* first multiple of 6 */

    while (true) {
        if (is_valid(cand)) {
            ++count;
            if (count == n)
                return (seq_t)cand;
        }
        cand += 6ULL;
    }
}
