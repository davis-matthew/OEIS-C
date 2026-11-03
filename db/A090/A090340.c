/*
 * A090340  Difference between the sums of the prime factors, including
 *          multiplicity, of n and those of n + 1.
 * https://oeis.org/A090340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sopfr(n) - sopfr(n+1) where sopfr(k) is the sum of prime
 *            factors of k counted with multiplicity
 * Range:     i8 n <= 86, i16 n <= 86, i32 n <= 86, i64 n <= 86, i128 n <= 86,
 *            big any n
 * Validated: partial, pass, 86 terms through n = 86 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the sum of prime factors of v with multiplicity.
   v is treated as an unsigned 64 bit integer. */
static seq_t sopfr_uint64(uint64_t v)
{
    seq_t sum = 0;
    if (v < 2) return sum;            /* sopfr(0) = sopfr(1) = 0 */

    while ((v & 1ULL) == 0) {          /* factor out 2's */
        sum += (seq_t)2;
        v >>= 1;
    }

    for (uint64_t p = 3; p * p <= v; p += 2) {
        while (v % p == 0) {
            sum += (seq_t)p;
            v /= p;
        }
    }

    if (v > 1) {                      /* remaining prime factor */
        sum += (seq_t)v;
    }

    return sum;
}

/* OEIS sequence A090340: difference between sopfr(n) and sopfr(n+1). */
seq_t a(idx_t n)
{
    /* Convert to 64‑bit value for factoring. */
    uint64_t u = (uint64_t)n;
    seq_t s_n   = sopfr_uint64(u);
    seq_t s_n1  = sopfr_uint64(u + 1);
    return s_n - s_n1;
}
