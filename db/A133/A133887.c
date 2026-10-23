/*
 * A133887  Binomial(n+7,n) mod 7^2.
 * https://oeis.org/A133887
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n+7,7) mod 49
 * Range:     u8 n <= 1500, u16 n <= 1500, u32 n <= 1500, u64 n <= 1500,
 *            u128 n <= 1500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence has period 343, so we reduce n modulo 343 to keep intermediate values small. */
#include <stdint.h>

seq_t a(idx_t n) {
    /* Period is 7^3 = 343. Reducing n keeps the product small. */
    idx_t m = n % 343U;

    /* Compute the binomial coefficient (m+7 choose 7) exactly using 128‑bit arithmetic. */
    unsigned __int128 prod = 1;
    /* Numerator product: (m+1)(m+2)...(m+7) */
    prod *= (unsigned __int128)(m + 1);
    prod *= (unsigned __int128)(m + 2);
    prod *= (unsigned __int128)(m + 3);
    prod *= (unsigned __int128)(m + 4);
    prod *= (unsigned __int128)(m + 5);
    prod *= (unsigned __int128)(m + 6);
    prod *= (unsigned __int128)(m + 7);

    /* 7! = 5040. Division is exact. */
    prod /= 5040U;

    /* Return the value modulo 49. */
    return (seq_t)(prod % 49U);
}
