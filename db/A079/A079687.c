/*
 * A079687  (C(p^2,p)-p)/p^5 where p runs through the primes >= 5.
 * https://oeis.org/A079687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (C(p^2,p) - p) / p^5, where p = prime(n+2). The
 *            calculation is performed using a multiplicative binomial
 *            algorithm; for 64‑bit builds the intermediate arithmetic is
 *            carried out in unsigned __int128 to avoid overflow, while for
 *            arbitrary‑precision builds the native seq_t type is used
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/get_prime.h"

#ifdef __cplusplus
#include <type_traits>
#endif

/* ------------------------------------------------------------------ */
/* Local helper: GCD for seq_t (works for any arithmetic sequence type) */
static seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ------------------------------------------------------------------ */
/* Local helper: GCD for unsigned __int128 (used in 64‑bit path) */
static unsigned __int128 gcd_uint128(unsigned __int128 a, unsigned __int128 b)
{
    while (b != (unsigned __int128)0) {
        unsigned __int128 t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) {
        return (seq_t)0;   /* out of range */
    }

    /* 1. Determine the prime p = prime(n+2). */
    idx_t p = get_prime(n + 2);

    /* 2. Compute N = p^2. */
    /* Two code paths: small seq_t (<= 8 bytes) and arbitrary‑precision. */
    if (sizeof(seq_t) <= 8) {
        /* 64‑bit build: use 128‑bit intermediates. */
        unsigned __int128 pp   = (unsigned __int128)p * (unsigned __int128)p;
        unsigned __int128 bin  = (unsigned __int128)1;
        unsigned __int128 num, den, g1, g2;

        for (idx_t i = 1; i <= p; ++i) {
            num = pp - (unsigned __int128)p + (unsigned __int128)i;
            den = (unsigned __int128)i;

            /* Reduce num/den by gcd to keep numbers small. */
            g1 = gcd_uint128(num, den);
            num /= g1;
            den /= g1;

            /* Reduce bin/den by gcd. */
            g2 = gcd_uint128(bin, den);
            bin /= g2;
            den /= g2;

            /* After reductions den should be 1. */
            bin = bin * num;   /* bin = bin * num / den  (den = 1) */
        }

        /* Now bin = C(p^2, p). Subtract p and divide by p^5. */
        unsigned __int128 result = (bin - (unsigned __int128)p);
        unsigned __int128 pow5   = (unsigned __int128)1;
        for (idx_t k = 0; k < 5; ++k) {
            pow5 *= (unsigned __int128)p;
        }
        result /= pow5;

        return (seq_t)result;
    } else {
        /* Arbitrary‑precision build: use seq_t operations directly. */
        seq_t N   = (seq_t)p * (seq_t)p;
        seq_t bin = (seq_t)1;

        for (idx_t i = 1; i <= p; ++i) {
            seq_t num = N - (seq_t)p + (seq_t)i;
            seq_t den = (seq_t)i;

            seq_t g1 = gcd_seq_t(num, den);
            num /= g1;
            den /= g1;

            seq_t g2 = gcd_seq_t(bin, den);
            bin /= g2;
            den /= g2;

            /* After reductions den should be 1. */
            bin = bin * num;   /* bin = bin * num / den  (den = 1) */
        }

        /* Now bin = C(p^2, p). Subtract p and divide by p^5. */
        seq_t pow5 = (seq_t)1;
        for (idx_t k = 0; k < 5; ++k) {
            pow5 *= (seq_t)p;
        }

        return (bin - (seq_t)p) / pow5;
    }
}
