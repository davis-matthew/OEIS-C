/*
 * A095825  Numbers k such that k + largest digit of k is a square.
 * https://oeis.org/A095825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) is the nth integer k such that k + largest digit of k is a
 *            perfect square. Computed by brute‑force search (incrementally
 *            generating terms)
 * Range:     u8 n <= 15, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Integer square root for unsigned long long: floor(sqrt(n)). */
static inline unsigned long long isqrt_ull(unsigned long long n)
{
    unsigned long long lo = 0, hi = 0xFFFFFFFFULL, ans = 0;
    while (lo <= hi) {
        unsigned long long mid = (lo + hi) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

/* Test whether k + (largest digit of k) is a perfect square. */
static bool is_valid_k(unsigned long long k)
{
    unsigned long long maxd = 0, t = k;
    while (t) {
        unsigned long long d = t % 10ULL;
        if (d > maxd) maxd = d;
        t /= 10ULL;
    }
    unsigned long long sum = k + maxd;
    unsigned long long root = isqrt_ull(sum);
    return root * root == sum;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Static storage for terms found so far. */
    static seq_t *terms = NULL;
    static size_t len = 0, cap = 0;

    /* Static variable holding the next integer to test. */
    static unsigned long long next_k = 0;

    /* Produce terms until we have at least n of them. */
    while (len < (size_t)n) {
        if (is_valid_k(next_k)) {
            /* Grow the array if needed. */
            if (len >= cap) {
                size_t new_cap = cap ? cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);  /* allocation failure */
                terms = tmp;
                cap = new_cap;
            }
            terms[len++] = (seq_t)next_k;  /* cast fits the tests for the build */
        }
        ++next_k;
    }

    return terms[n - 1];
}
