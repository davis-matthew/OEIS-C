/*
 * A135724  Fibonacci numbers whose indices are prime Fibonacci numbers: a(n)
 *          = Fibonacci(Fibonacci(A001605(n))).
 * https://oeis.org/A135724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(Fibonacci(A001605(n)))
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Validated: standard, pass, 7 terms through n = 7 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A135724.                      */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

static seq_t fib(idx_t n) {
    if (n <= 2) return (seq_t)1;
    seq_t f1 = 1, f2 = 1, f3;
    for (idx_t i = 3; i <= n; ++i) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }
    return f2;
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
    /* n >= 1 is assumed. */
    idx_t count = 0;
    seq_t prev2 = 1; /* F(1) */
    seq_t prev1 = 1; /* F(2) */
    seq_t result = 0;

    for (idx_t i = 3; ; ++i) {
        seq_t f = prev2 + prev1;  /* F(i) = F(i-1) + F(i-2) */
        prev2 = prev1;
        prev1 = f;

        if (is_prime_big(f)) {
            /* f fits into idx_t because it is a known prime Fibonacci number
               whose value is <= 2^64-1 for all entries that the sequence
               contains. */
            result = fib((idx_t)f);
            ++count;
            if (count == n)
                break;
        }
    }

    return result;
}
