/*
 * A076888  a(n) is the number of divisors of the n-th positive palindromic
 *          number.
 * https://oeis.org/A076888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = d(A002113(n)) (number of divisors of the n‑th palindrome)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Reverse the decimal digits of a positive integer. */
static uint64_t reverse_uint64(uint64_t x)
{
    uint64_t r = 0;
    while (x) {
        r = r * 10 + (x % 10);
        x /= 10;
    }
    return r;
}

/* Compute 10^exp for exp in 0..18, using only integer arithmetic. */
static uint64_t pow10_u64(uint32_t exp)
{
    uint64_t r = 1;
    while (exp--) r *= 10ULL;
    return r;
}

/* Number of palindromes of exact length L (base 10). */
static uint64_t pal_cnt_len(uint64_t L)
{
    /* 9 * 10^{floor((L-1)/2)} */
    return 9ULL * pow10_u64((L - 1) / 2);
}

/* Return the nth palindrome (1‑based). */
static uint64_t nth_palindrome(idx_t n)
{
    uint64_t cum = 0;
    uint64_t L = 1;
    while (1) {
        uint64_t cnt = pal_cnt_len(L);
        if (cum + cnt >= n) break;
        cum += cnt;
        ++L;
    }
    uint64_t offset = n - cum;                 /* 1‑based position inside length L block */
    uint64_t half_len = (L + 1) / 2;            /* number of digits in the first half */
    uint64_t base = pow10_u64(half_len - 1) + offset - 1; /* first half + offset */

    uint64_t num;
    if (L % 2 == 0) {
        /* even length: mirror whole base */
        num = base * pow10_u64(half_len) + reverse_uint64(base);
    } else {
        /* odd length: drop middle digit before mirroring */
        uint64_t mid_removed = base / 10;
        num = base * pow10_u64(half_len - 1) + reverse_uint64(mid_removed);
    }
    return num;
}

/* Count divisors of n (uint64_t) by trial division. */
static uint64_t divisor_count_u64(uint64_t n)
{
    if (n == 0) return 0;        /* unused, but keeps function safe */
    uint64_t count = 1;
    uint64_t temp = n;

    /* factor 2 */
    int exp = 0;
    while ((temp & 1ULL) == 0) {
        temp >>= 1;
        ++exp;
    }
    if (exp) count *= (exp + 1);

    /* odd factors */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            count *= (exp + 1);
        }
    }

    if (temp > 1) count *= 2;    /* remaining prime factor */
    return count;
}

seq_t a(idx_t n)
{
    uint64_t pal = nth_palindrome(n);
    return (seq_t)divisor_count_u64(pal);
}
