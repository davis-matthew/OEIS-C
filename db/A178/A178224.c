/*
 * A178224  Numbers k such that d(1)^1 + d(2)^2 +... + d(p)^p is a square,
 *          where d(i), i=1..p, are the decimal digits of k.
 * https://oeis.org/A178224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 470, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A178224:  Numbers k such that the sum
 *  d(1)^1 + d(2)^2 + … + d(p)^p of the decimal digits of k
 *  (with d(1) being the most significant digit) is a perfect square.
 *
 *  a(1) = 0, a(2) = 1, a(3) = 4, …  (see OEIS entry for full list)
 */

#include <stdlib.h>
#include <stdint.h>

/* Integer square root for 64‑bit unsigned values (binary search) */
static uint64_t isqrt_u64(uint64_t x) {
    uint64_t lo = 0, hi = 1ULL << 32, mid, res = 0;
    if (x < 2) return x;
    while (lo <= hi) {
        mid = (lo + hi) >> 1;
        uint64_t m2 = mid * mid;
        if (m2 == x) return mid;
        if (m2 < x) {
            lo = mid + 1;
            res = mid;
        } else {
            hi = mid - 1;
        }
    }
    return res;
}

/* Grow a dynamic array of seq_t values.  The array is managed
 * with a capacity pointer that is doubled when needed. */
static void ensure_capacity(seq_t **arr, size_t *cap, size_t need) {
    if (need <= *cap) return;
    size_t new_cap = (*cap == 0) ? 16 : *cap * 2;
    while (new_cap < need) new_cap *= 2;
    *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    /* Static cache of computed terms. */
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static uint64_t next_k = 0;          /* next integer to test */

    /* Generate terms until we have at least n entries. */
    while (terms_len < n) {
        uint64_t k = next_k;

        /* ----------   Extract decimal digits   ---------- */
        uint8_t digits[32];   /* enough for 64‑bit numbers */
        size_t dcnt = 0;
        uint64_t tmp = k;
        if (tmp == 0) {
            digits[dcnt++] = 0;
        } else {
            while (tmp != 0) {
                digits[dcnt++] = (uint8_t)(tmp % 10);
                tmp /= 10;
            }
            /* reverse to get most significant digit first */
            for (size_t i = 0; i < dcnt / 2; ++i) {
                uint8_t swap = digits[i];
                digits[i] = digits[dcnt - 1 - i];
                digits[dcnt - 1 - i] = swap;
            }
        }

        /* ----------   Compute Σ d(i)^i   ---------- */
        seq_t sum = 0;
        for (size_t pos = 1; pos <= dcnt; ++pos) {
            uint8_t d = digits[pos - 1];
            seq_t pow = 1;
            for (size_t e = 0; e < pos; ++e) pow *= d;
            sum += pow;
        }

        /* ----------   Check if sum is a square   ---------- */
        uint64_t sum_u64 = (uint64_t)sum;          /* safe: sum fits in 64 bits */
        uint64_t root = isqrt_u64(sum_u64);
        if (root * root == sum_u64) {
            /* Store this term in the cache. */
            ensure_capacity(&terms, &terms_cap, terms_len + 1);
            terms[terms_len++] = (seq_t)k;
        }
        ++next_k;
    }

    /* ----------   Return the nth term (1‑based)   ---------- */
    return terms[n - 1];
}
