/*
 * A095274  a(n) = (A095100(n)-3)/4.
 * https://oeis.org/A095274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A095100(n+1)-3)/4, where A095100 lists the m = 4k+3
 *            whose Jacobi-symbol prefix sums J(i/m), i=1..u, are all >= 0;
 *            computed by a search over m
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 66 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* SHARED_HELPER: Jacobi symbol (a/n) for odd n */
static int A095274_jacobi(uint64_t a, uint64_t n)
{
    if (n <= 1) return 0;
    a %= n;
    int s = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            if ((n & 7) == 3 || (n & 7) == 5) s = -s;
        }
        uint64_t t = a; a = n; n = t;
        if ((a & 3) == 3 && (n & 3) == 3) s = -s;
        a %= n;
    }
    return (n == 1) ? s : 0;
}

static bool A095274_valid(uint64_t m)
{
    int64_t sum = 0;
    for (uint64_t i = 1; i < m; ++i) {
        sum += A095274_jacobi(i, m);
        if (sum < 0) return false;
    }
    return true;
}

static uint64_t *A095274_terms = NULL;
static size_t A095274_len = 0, A095274_cap = 0;
static uint64_t A095274_next = 0; /* next k, m = 4k+3 */

seq_t a(idx_t n)
{
    while (A095274_len <= (size_t)n) {
        uint64_t k = A095274_next++;
        if (A095274_valid(4 * k + 3)) {
            if (A095274_len == A095274_cap) {
                size_t nc = A095274_cap ? A095274_cap * 2 : 64;
                uint64_t *t = (uint64_t *)realloc(A095274_terms, nc * sizeof(uint64_t));
                if (!t) abort();
                A095274_terms = t;
                A095274_cap = nc;
            }
            A095274_terms[A095274_len++] = k;
        }
    }
    return (seq_t)A095274_terms[(size_t)n];
}
