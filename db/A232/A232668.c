/*
 * A232668  Natural numbers that are not (primes, 11-smooth, perfect powers or
 *          base-10 palindromes).
 * https://oeis.org/A232668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the n-th natural number that is NOT a prime, NOT an
 *            11‑smooth number, NOT a perfect power, and NOT a base‑10
 *            palindrome
 * Range:     u8 n <= 45, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-03)
 * Helpers:   bit_length, ipow_u64_idx, is_palindrome_base, is_prime_u64, mem, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/bit_length.h"
#include "../utils/ipow_u64_idx.h"
#include "../utils/is_palindrome_base.h"
#include "../utils/pow2.h"
#include "../utils/mem.h"
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

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

/* Helper: returns true if x has only prime factors <= 11. */
static bool is_11smooth(seq_t x)
{
    static const int smooth_primes[5] = {2, 3, 5, 7, 11};
    seq_t tmp = x;
    for (int i = 0; i < 5; ++i) {
        int p = smooth_primes[i];
        while (tmp % p == 0)
            tmp /= p;
    }
    return tmp == 1;
}

/* Helper: integer k‑th root of n, floor. */
static seq_t nth_root(seq_t n, int k)
{
    seq_t low  = 1;
    seq_t high = pow2((bit_length(n) + k - 1) / k + 1);
    seq_t ans  = 1;
    while (low <= high) {
        seq_t mid = (low + high) >> 1;
        seq_t val = ipow_u64_idx((uint64_t)mid, k);
        if (val == n)
            return mid;
        if (val < n) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

/* Helper: true if x is a perfect power (m^k, m>0, k>=2). */
static bool is_perfect_power(seq_t x)
{
    if (x <= 3)  /* 2^2 = 4 is first perfect power */
        return false;

    unsigned bits = bit_length(x);
    for (int k = 2; k <= (int)bits; ++k) {
        seq_t r = nth_root(x, k);
        if (r <= 1)
            continue;
        if (ipow_u64_idx((uint64_t)r, k) == x)
            return true;
    }
    return false;
}

/* Helper: true if x belongs to any excluded category. */
static bool is_excluded(seq_t x)
{
    if (x < 2)
        return true;                 /* 1 is a palindrome anyway */

    if (is_prime_big(x))
        return true;                 /* prime */

    if (is_11smooth(x))
        return true;                 /* 11‑smooth */

    if (is_palindrome_base(x, 10))
        return true;                 /* base‑10 palindrome */

    if (is_perfect_power(x))
        return true;                 /* perfect power */

    return false;
}

/* Main function implementing the sequence. */
seq_t a(idx_t n)
{
    static seq_t *seq_arr = NULL;
    static size_t seq_len = 0;
    static size_t seq_cap = 0;
    static seq_t next_val = 2;            /* start checking from 2 */

    while (seq_len < n) {
        if (!is_excluded(next_val)) {
            grow_seq_array(&seq_arr, &seq_cap, seq_len + 1);
            seq_arr[seq_len++] = next_val;
        }
        ++next_val;
    }
    return seq_arr[n - 1];
}
