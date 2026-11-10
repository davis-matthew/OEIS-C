/*
 * A086909  Middle side of the first primitive d-arithmetic triangle, where
 *          d=A072330(n).
 * https://oeis.org/A086909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   with d = A072330(n), a(n) = 2x for the least x > d with
 *            gcd(x,d)=1 and (x^2-d^2)/3 a perfect square (search)
 * Range:     u8 n <= 5, u16 n <= 25, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 69 terms through n = 1000 (2026-11-07)
 * Helpers:   gcd, is_square, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"
#include "../utils/gcd.h"
#include "../utils/is_square.h"

static bool A086909_allowed(idx_t x)
{
    if (x <= 1) return true;
    for (idx_t p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            idx_t r = p % 12;
            if (r != 1 && r != 11) return false;
            while (x % p == 0) x /= p;
        }
    }
    if (x > 1) {
        idx_t r = x % 12;
        if (r != 1 && r != 11) return false;
    }
    return true;
}

static idx_t *A086909_ds = NULL;
static size_t A086909_len = 0, A086909_cap = 0;
static idx_t A086909_last = 1;

static idx_t A086909_d(idx_t n)
{
    if (!A086909_ds) {
        A086909_cap = 64;
        A086909_ds = (idx_t *)malloc(A086909_cap * sizeof(idx_t));
        if (!A086909_ds) exit(1);
        A086909_ds[0] = 1;
        A086909_len = 1;
    }
    while (A086909_len < n) {
        ++A086909_last;
        if (A086909_allowed(A086909_last)) {
            if (A086909_len == A086909_cap) {
                A086909_cap *= 2;
                A086909_ds = (idx_t *)realloc(A086909_ds, A086909_cap * sizeof(idx_t));
                if (!A086909_ds) exit(1);
            }
            A086909_ds[A086909_len++] = A086909_last;
        }
    }
    return A086909_ds[n - 1];
}

seq_t a(idx_t n)
{
    seq_t d = (seq_t)A086909_d(n);
    seq_t dd = d * d;
    for (seq_t x = d + 1;; ++x) {
        seq_t t = x * x - dd;
        if (t % 3 != 0) continue;
        if (!is_square(t / 3)) continue;
        if (gcd(x, d) != 1) continue;
        return x * 2;
    }
}
