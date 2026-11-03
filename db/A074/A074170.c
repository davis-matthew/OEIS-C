/*
 * A074170  Start with 1, add the next number if one gets a prime then add the
 *          next number else subtract the next...
 * https://oeis.org/A074170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(1) = 1; for n > 1, if a(n‑1) == 1 or |a(n‑1)| is prime then
 *            a(n) = a(n‑1) + n, else a(n) = a(n‑1) – n
 * Range:     i8 n <= 20, i16 n <= 56, i32 n <= 56, i64 n <= 56, i128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

/* absolute value for seq_t (avoids overflow for small values) */
static inline seq_t abs_seq_t(seq_t x)
{
    return (x >= 0) ? x : -x;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;

    seq_t v = 1; /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        bool prime = false;
        if (v == 1)                /* treat the initial 1 as "prime" */
            prime = true;
        else
        {
            seq_t abs_v = abs_seq_t(v);
            /* is_prime_big considers only positive numbers; abs_v ≥ 2 here */
            prime = is_prime_big(abs_v);
        }

        if (prime)
            v = v + (seq_t)i;
        else
            v = v - (seq_t)i;
    }
    return v;
}
