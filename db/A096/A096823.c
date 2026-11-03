/*
 * A096823  a(n) = p*(p+(2n-1))/2, where p = A096822(n) is the smallest primes
 *          of form 2^x-(2n-1).
 * https://oeis.org/A096823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = p * (p + (2n-1)) / 2, where p is the smallest prime of
 * the form 2^x - (2n-1).
 *
 * The computed value can be expressed as a(n) = p * 2^(x-1).
 */

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

/* Small helper: floor(log2(n)) for non‑zero unsigned integers. */
static inline idx_t floor_log2_uint64(idx_t n)
{
#if defined(__GNUC__) || defined(__clang__)
    /* GCC/Clang built‑in, defined for GCC and Clang compliant C/C++. */
    return n == 0 ? 0 : (sizeof(idx_t) * 8 - 1 - __builtin_clzll(n));
#else
    idx_t r = 0;
    while ((idx_t)1 << (r + 1) <= n) ++r;
    return r;
#endif
}

/* Small helper: 2^exp as seq_t. */
static inline seq_t pow2_seq(idx_t exp)
{
    return ((seq_t)1) << exp;
}

seq_t a(idx_t n)
{
    /* a(1) is the first term; behaviour for n < 1 is irrelevant. */
    if (n < 1u) {
        return (seq_t)0;
    }

    /* Target t = 2n-1.  We need p = 2^x - t to be prime. */
    idx_t target = (idx_t)2 * n - (idx_t)1;

    /* Start from the smallest exponent x such that 2^x > target. */
    idx_t x = floor_log2_uint64(target) + (idx_t)1;

    /* Search for the smallest x with prime p. */
    for (;;) {
        seq_t p = pow2_seq(x) - (seq_t)target;

        if (is_prime_big(p)) {
            /* a(n) = p * 2^(x-1). */
            seq_t a_val = p * pow2_seq(x - (idx_t)1);
            return a_val;
        }
        ++x;
    }
}
