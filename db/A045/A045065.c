/*
 * A045065  Numbers whose base-4 representation contains no 0's and exactly
 *          four 3's.
 * https://oeis.org/A045065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th positive integer whose base-4 representation
 *            contains no 0s and exactly four 3s
 * Range:     u8 n <= 1, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Validated: partial, pass, 38 terms through n = 38 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* C(n,k) capped at cap (returns cap+1 if the exact value would exceed cap). */
static inline idx_t comb_capped(idx_t n, idx_t k, idx_t cap) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    unsigned __int128 res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        unsigned __int128 numerator = res * (n - k + i);
        unsigned __int128 denominator = i;
        res = numerator / denominator;
        if (res > cap) return cap + 1;
    }
    return (idx_t)res;
}

/* Multiply a and b capped at cap (returns cap+1 if the product would exceed cap). */
static inline idx_t mul_capped(idx_t a, idx_t b, idx_t cap) {
    if (a == 0 || b == 0) return 0;
    if (a > cap / b) return cap + 1;
    return a * b;
}

/* 2^e capped at cap (returns cap+1 if the exact value would exceed cap). */
static inline idx_t pow2_capped(idx_t e, idx_t cap) {
    if (e == 0) return (cap >= 1) ? 1 : cap + 1;
    if (e >= 64) return cap + 1;
    unsigned __int128 res = 1;
    for (idx_t i = 0; i < e; ++i) {
        res <<= 1;
        if (res > cap) return cap + 1;
    }
    return (idx_t)res;
}

/* Main sequence generator. */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined for n < 1 */

    /* Determine the number of digits d for the n-th term. */
    idx_t remaining = n;
    idx_t d = 4;
    while (1) {
        idx_t cap = remaining;
        idx_t count_d = mul_capped(
            comb_capped(d, 4, cap),
            pow2_capped(d - 4, cap),
            cap
        );
        if (count_d == 0 || count_d >= remaining) {
            break;
        }
        remaining -= count_d;
        ++d;
    }

    /* remaining is now the index (1‑based) within the numbers of length d. */
    idx_t k = remaining;
    idx_t t = 4;                     /* remaining 3’s to place */
    seq_t value = 0;

    for (idx_t pos = 0; pos < d; ++pos) {
        idx_t r = d - pos;          /* positions still free */

        /* Try digit 1. */
        idx_t cnt;
        if (t > r - 1)
            cnt = 0;
        else {
            idx_t cap = k - 1;
            cnt = mul_capped(
                comb_capped(r - 1, t, cap),
                pow2_capped(r - 1 - t, cap),
                cap
            );
        }
        if (k <= cnt) {
            value = value * 4 + (seq_t)1;
            continue;
        }
        k -= cnt;

        /* Try digit 2 (same count as 1). */
        if (t > r - 1)
            cnt = 0;
        else {
            idx_t cap = k - 1;
            cnt = mul_capped(
                comb_capped(r - 1, t, cap),
                pow2_capped(r - 1 - t, cap),
                cap
            );
        }
        if (k <= cnt) {
            value = value * 4 + (seq_t)2;
            continue;
        }
        k -= cnt;

        /* Remaining digit must be 3. */
        value = value * 4 + (seq_t)3;
        --t;
    }

    return value;
}
