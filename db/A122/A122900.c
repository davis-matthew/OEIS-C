/*
 * A122900  Minimum prime of the form n^k + (n+1)^k for k>1, or 0 if no such
 *          prime exists.
 * https://oeis.org/A122900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  hard, nonn
 * Formula:   a(n) = minimum prime of n^k + (n+1)^k for k>1, otherwise 0
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 10 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_u64.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
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

seq_t a(idx_t n) {
    /* Convert the index to the sequence type. */
    seq_t nseq  = (seq_t)n;
    seq_t nseq1 = nseq + 1;

    /* Small exponents: 2 .. 64 (inclusive).  We use incremental
       multiplication so each step needs only one big multiplication. */
    seq_t pow_n  = nseq;
    seq_t pow_n1 = nseq1;
    for (idx_t k = 2; k <= 64; ++k) {
        pow_n  = seq_mul(pow_n, nseq);   /* n^k  */
        pow_n1 = seq_mul(pow_n1, nseq1); /* (n+1)^k */
        seq_t sum = pow_n + pow_n1;
        if (is_prime_big(sum))
            return sum;  /* The first prime is the minimal one. */
    }

    /* Larger exponents that are powers of two: 128, 256, 512, 1024. */
    for (idx_t exp = 128; exp <= 1024; exp <<= 1) {
        pow_n  = seq_mul(pow_n,  pow_n);   /* n^exp  */
        pow_n1 = seq_mul(pow_n1, pow_n1);  /* (n+1)^exp */
        seq_t sum = pow_n + pow_n1;
        if (is_prime_big(sum))
            return sum;
    }

    /* No prime found within the tested range. */
    return (seq_t)0;
}
