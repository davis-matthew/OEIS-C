/*
 * A029914  Start with n; repeatedly sum squares of prime factors (counted
 *          with multiplicity), until reach a prime p, then set a(n) = p; if
 *          reach a fixed point q, set a(n) = q; set a(n) = 0 if no limit
 *          exists.
 * https://oeis.org/A029914
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 5 terms through n = 6 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A029914.
 * Start with n; repeatedly replace by the sum of squares of its prime
 * factors (counted with multiplicity).  Stop when the result is a prime
 * p, then return p;  if a fixed point q is reached, return q;
 * otherwise return 0 after a reasonable number of iterations. */

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

/* Helper: sum of squares of prime factors of x (with multiplicity). */
static seq_t sum_prime_factor_squares(seq_t x)
{
    seq_t temp = x;
    seq_t sum  = 0;

    /* Trial division using an index counter up to sqrt(temp).  This loop
       works for all supported seq_t sizes because the comparison uses '/'.
       The loop terminates when the current potential divisor p satisfies
       p > temp/p, which is equivalent to p*p > temp and therefore p
       > sqrt(temp). */
    for (idx_t i = 2; ; ++i) {
        seq_t p = (seq_t)i;
        if (p > temp / p) {
            break;                /* no further factors possible */
        }
        while (temp % p == 0) {
            sum += p * p;
            temp /= p;
        }
    }
    /* If temp is > 1 it is a prime factor left over. */
    if (temp > 1) {
        sum += temp * temp;
    }
    return sum;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 2) {
        return 0;                 /* sequence offset starts at 2 */
    }

    seq_t current = (seq_t)n;
    const int MAX_STEPS = 1024;

    for (int step = 0; step < MAX_STEPS; ++step) {
        seq_t next = sum_prime_factor_squares(current);
        if (is_prime_big(next)) {
            return next;          /* reached a prime */
        }
        if (next == current) {
            return next;          /* fixed point */
        }
        current = next;
    }
    return 0;                     /* no limit found within step bound */
}
