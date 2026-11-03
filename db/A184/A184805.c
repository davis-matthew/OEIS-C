/*
 * A184805  Primes of the form floor(k*s), where s=(5+sqrt(5))/4.
 * https://oeis.org/A184805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Validated: partial, pass, 99 terms through n = 99 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence a(n) is defined as the n-th prime of the Beatty sequence
   floor(k * s) where s = (5 + sqrt(5)) / 4.  The function below
   computes a(n) for any n >= 1 by iterating over k, evaluating
   floor(k*s) exactly with integer arithmetic, testing the result
   for primality, and returning the n-th prime encountered. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/is_prime_u64.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
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

/* Compute floor(k * s) with s = (5 + sqrt(5)) / 4, using only integer
   arithmetic.  For an integer k,
        k * s = (5k + k * sqrt(5)) / 4
   and floor(k * sqrt(5)) = floor(sqrt(5 * k^2)). */
static seq_t floor_k_s(idx_t k)
{
    seq_t k_seq = (seq_t)k;                    /* convert k to seq_t */
    seq_t k_sq   = k_seq * k_seq;              /* k^2                      */
    seq_t tmp    = k_sq * (seq_t)5;            /* 5 * k^2                  */
    seq_t sqrt_k = floor_sqrt(tmp);            /* floor(k * sqrt(5))       */
    seq_t num    = k_seq * (seq_t)5 + sqrt_k;  /* 5k + floor(k*sqrt(5))    */
    return num / (seq_t)4;                     /* floor division by 4      */
}

/* Return the n-th term of the sequence (1‑based).  The function
   iterates over k, evaluates floor(k*s), checks primality,
   and returns the n‑th prime found. */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* The sequence is defined for n >= 1; return zero for safety. */
        return (seq_t)0;
    }

    idx_t count = 0;
    idx_t k = 1;

    while (true) {
        seq_t val = floor_k_s(k);
        if (is_prime_big(val)) {
            ++count;
            if (count == n) {
                return val;
            }
        }
        ++k;
    }
}
