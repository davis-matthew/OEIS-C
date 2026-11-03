/*
 * A215453  a(n) = least k>0 such that n^n divides Fibonacci(k).
 * https://oeis.org/A215453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 852 (2026-11-01)
 * Helpers:   gcd, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Compute a(n) = least k>0 such that n^n divides Fibonacci(k).
 * The routine factors n, finds the minimal k for each prime power p^e
 * using known rank formulas, and returns the lcm of those k's.
 * All arithmetic uses the provided seq_t type (uint8_t, uint16_t, …,
 * unsigned __int128, or arbitrary precision).
 */

#include <stdbool.h>

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/ipow_idx.h"

/* minimal k such that prime p divides Fibonacci(k) */
static seq_t rank_prime(uint64_t p)
{
    seq_t mod = (seq_t)p;
    seq_t a = (seq_t)1;   /* Fib(1) */
    seq_t b = (seq_t)1;   /* Fib(2) */
    seq_t k = 3;

    for (;;) {
        seq_t c = (a + b) % mod;
        a = b;
        b = c;
        if (b == (seq_t)0)
            break;
        ++k;
    }
    return k;
}

/* minimal k such that p^e divides Fibonacci(k) */
static seq_t rank_prime_power(uint64_t p, idx_t e)
{
    if (p == 2ULL) {
        if (e == 1)
            return (seq_t)3;
        if (e == 2)
            return (seq_t)6;
        /* e >= 3 : rank = 3 * 2^(e-2) */
        seq_t pow2 = ipow_idx((seq_t)2, e - 2); /* 2^(e-2) */
        return (seq_t)3 * pow2;
    } else {
        seq_t rp = rank_prime(p);
        if (e == 1)
            return rp;
        seq_t powp = ipow_idx((seq_t)p, e - 1); /* p^(e-1) */
        return rp * powp;
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)n;  /* a(1)=1; a(0) unused but returns 0 */

    seq_t result = (seq_t)1;
    uint64_t tmp = (uint64_t)n;

    /* handle factor 2 separately */
    if (tmp % 2ULL == 0) {
        idx_t exp = 0;
        while (tmp % 2ULL == 0) {
            ++exp;
            tmp /= 2ULL;
        }
        idx_t e_total = exp * n;
        seq_t k_min = rank_prime_power(2ULL, e_total);
        seq_t g = gcd(result, k_min);
        result = (result / g) * k_min;
    }

    /* odd primes */
    for (uint64_t p = 3ULL; p * p <= tmp; p += 2ULL) {
        if (tmp % p == 0) {
            idx_t exp = 0;
            while (tmp % p == 0) {
                ++exp;
                tmp /= p;
            }
            idx_t e_total = exp * n;
            seq_t k_min = rank_prime_power(p, e_total);
            seq_t g = gcd(result, k_min);
            result = (result / g) * k_min;
        }
    }

    /* remaining prime factor > sqrt(original n) */
    if (tmp > 1ULL) {
        idx_t exp = 1;
        idx_t e_total = exp * n;
        seq_t k_min = rank_prime_power(tmp, e_total);
        seq_t g = gcd(result, k_min);
        result = (result / g) * k_min;
    }

    return result;
}
