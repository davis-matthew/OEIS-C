/*
 * A177941  a(1)=5; for n>0, a(n+1)=a(n)+p-1, where p is the smallest prime
 *          divisor of (a(n))^2-4.
 * https://oeis.org/A177941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A177941:
 * a(1) = 5;
 * a(n+1) = a(n) + p - 1,
 * where p is the smallest prime divisor of a(n)^2 – 4.
 * Since a(n)^2 – 4 = (a(n) – 2)(a(n) + 2),
 * p is the smaller of the smallest prime divisors of a(n) – 2 and a(n) + 2. */
#include <stdint.h>

/* Smallest prime factor of a 64‑bit integer (value ≥ 2).
 * Uses trial division with 6k±1 optimisation. */
static uint64_t spf_uint64(uint64_t n)
{
    if ((n & 1ULL) == 0ULL) return 2ULL;      /* even */
    if (n % 3ULL == 0ULL) return 3ULL;        /* divisible by 3 */
    for (uint64_t i = 5ULL; i <= n / i; i += 6ULL) {
        if (n % i == 0ULL) return i;
        if (n % (i + 2ULL) == 0ULL) return i + 2ULL;
    }
    return n; /* n itself is prime */
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* undefined for n==0, return 0 */

    seq_t val = 5ULL;                    /* a(1) */
    for (idx_t i = 1; i < n; ++i) {
        /* Compute the smallest prime factors of (a-2) and (a+2) .
           The sequence values stay below 2^64 for the tested range,
           thus the casts to uint64_t are safe. */
        uint64_t left  = (uint64_t)(val - 2);
        uint64_t right = (uint64_t)(val + 2);

        uint64_t p_left  = spf_uint64(left);
        uint64_t p_right = spf_uint64(right);
        uint64_t p = (p_left < p_right) ? p_left : p_right;

        val = val + (seq_t)(p - 1);
    }
    return val;
}
