/*
 * A077558  Triangle in which the n-th row contains n numbers beginning with n
 *          that have the same prime signature as n.
 * https://oeis.org/A077558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) computed by searching for numbers with the same prime signature
   as the row number n in the triangle defined in OEIS A077558. */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Test two signatures for equality. */
static bool equal_sig(const uint8_t a[32], uint8_t la,
                      const uint8_t b[32], uint8_t lb)
{
    if (la != lb) return false;
    return memcmp(a, b, la) == 0;
}

/* Compute the multiset of prime‑factor exponents of m,
   sorted non‑decreasingly in sig[0..len-1]. */
static void signature(uint64_t m, uint8_t sig[32], uint8_t *len)
{
    *len = 0;
    uint64_t t = m;
    uint8_t e;

    /* factor 2 */
    e = 0;
    while ((t & 1ULL) == 0) {
        t >>= 1;
        ++e;
    }
    if (e) sig[(*len)++] = e;

    /* odd factors */
    for (uint64_t p = 3; p * p <= t; p += 2) {
        if (t % p == 0) {
            e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            sig[(*len)++] = e;
        }
    }
    if (t > 1) {
        /* remaining prime factor has exponent 1 */
        sig[(*len)++] = 1;
    }

    /* insertion sort to have the exponents in ascending order */
    for (int i = 1; i < *len; ++i) {
        uint8_t key = sig[i];
        int j = i - 1;
        while (j >= 0 && sig[j] > key) {
            sig[j + 1] = sig[j];
            --j;
        }
        sig[j + 1] = key;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* only defined for n≥1 */

    /* Find the row r: smallest r with r*(r+1)/2 ≥ n */
    idx_t low = 1, high = n;                    /* high is always enough */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        idx_t tri = mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;
    idx_t prev_tri = r * (r - 1) / 2;
    idx_t k = n - prev_tri;                    /* position within the row (1‑based) */

    /* Signature of the row number r */
    uint8_t r_sig[32], r_len;
    signature((uint64_t)r, r_sig, &r_len);

    /* Scan upward from r until we have found the k‑th value with the
       same prime signature. */
    idx_t val = r;
    idx_t count = 0;
    for (;; ++val) {
        uint8_t v_sig[32], v_len;
        signature((uint64_t)val, v_sig, &v_len);
        if (equal_sig(r_sig, r_len, v_sig, v_len)) {
            ++count;
            if (count == k)
                return (seq_t)val;
        }
    }
}
