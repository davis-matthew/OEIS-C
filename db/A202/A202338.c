/*
 * A202338  Smallest Fibonacci number which when appended to n produces a
 *          prime number.
 * https://oeis.org/A202338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 78 (2026-11-03)
 * Helpers:   is_prime_u64, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation searches for the smallest Fibonacci number f such
 * that the decimal concatenation of n and f is prime.
 * It generates Fibonacci numbers iteratively and checks each candidate
 * by constructing the concatenated number and using the provided
 * is_prime_big routine. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/num_digits.h"
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

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* start with the first two Fibonacci numbers 1,1 */
    seq_t f_prev = 1;   /* F(1) */
    seq_t f_curr = 1;   /* F(2) */

    /* test the very first Fibonacci number (1) */
    seq_t pow_w = pow10((idx_t)num_digits(f_prev));
    seq_t concat = pow_w * (seq_t)n + f_prev;
    if (is_prime_big(concat))
        return f_prev;

    /* iterate over subsequent Fibonacci numbers */
    while (true) {
        /* check current Fibonacci number */
        pow_w = pow10((idx_t)num_digits(f_curr));
        concat = pow_w * (seq_t)n + f_curr;
        if (is_prime_big(concat))
            return f_curr;

        /* compute next Fibonacci number */
        seq_t f_next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = f_next;
    }
}
