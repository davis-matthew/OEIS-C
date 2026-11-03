/*
 * A076887  Sum of divisors of nonzero palindromic numbers.
 * https://oeis.org/A076887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   a(n) = A000203(A002113(n+1))
 * Range:     u8 n <= 25, u16 n <= 330, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 8108 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return the nth non‑zero decimal palindrome (n≥1). */
static uint64_t nth_palindrome(idx_t n) {
    uint64_t count = 0;
    int len = 0;

    /* find length of the nth palindrome */
    for (;;) {
        ++len;
        int k = (len + 1) / 2;          /* number of free digits */
        uint64_t per_len = 9;
        for (int i = 1; i < k; ++i)
            per_len *= 10;
        if (count + per_len >= n)
            break;
        count += per_len;
    }

    uint64_t offset = (uint64_t)n - count - 1;  /* 0‑based within this length */
    int k = (len + 1) / 2;
    uint64_t begin = 1;
    for (int i = 1; i < k; ++i)
        begin *= 10;                    /* 10^(k‑1) */

    uint64_t first_half = begin + offset;

    /* build palindrome from first_half */
    uint64_t pal = first_half;
    uint64_t rev = first_half;
    if (len % 2 == 1)           /* omit middle digit for odd length */
        rev /= 10;
    while (rev > 0) {
        pal = pal * 10 + (rev % 10);
        rev /= 10;
    }
    return pal;
}

/* Compute sum of divisors σ(n) for n ≤ 10^12 (fits in uint64_t). */
static uint64_t sigma_uint64(uint64_t n) {
    uint64_t res = 1;
    uint64_t m = n;

    /* factor 2 */
    if ((m & 1ULL) == 0) {
        uint64_t sum = 1, pow = 1;
        while ((m & 1ULL) == 0) {
            m >>= 1;
            pow <<= 1;
            sum += pow;
        }
        res *= sum;
    }

    /* factor odd primes */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            uint64_t sum = 1, pow = 1;
            while (m % p == 0) {
                m /= p;
                pow *= p;
                sum += pow;
            }
            res *= sum;
        }
    }

    if (m > 1)          /* remaining prime factor */
        res *= (1 + m);

    return res;
}

seq_t a(idx_t n) {
    uint64_t pal = nth_palindrome(n);
    uint64_t sigma_val = sigma_uint64(pal);
    return (seq_t)sigma_val;
}
