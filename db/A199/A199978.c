/*
 * A199978  Nonprime numbers whose multiplicative digital root is 0.
 * https://oeis.org/A199978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th nonprime number whose multiplicative digital root is
 *            0
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-03)
 * Helpers:   digit_product, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function searches for the n-th positive integer that is not prime and has a multiplicative digital
 * root equal to 0. The search proceeds sequentially over increasing integers and stops when the n-th such
 * integer has been found. It uses the provided digit_product and is_prime_big utilities. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_product.h"
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

/* Return true if the multiplicative digital root of x is 0. */
static inline bool mult_root_zero(seq_t x)
{
    while (1)
    {
        /* If x has become a single digit, the root is x itself. */
        if (x < (seq_t)10)
            return x == (seq_t)0;

        seq_t prod = digit_product(x);
        if (prod == (seq_t)0)
            return true;

        x = prod;
    }
}

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t num   = 2;                 /* start from the first positive integer > 1 */

    while (count < n)
    {
        seq_t val = (seq_t)num;

        if (!is_prime_big(val) && mult_root_zero(val))
        {
            ++count;
            if (count == n)
                return val;
        }

        ++num;
    }

    /* The function always returns inside the loop for a valid n. */
    return (seq_t)0;
}
