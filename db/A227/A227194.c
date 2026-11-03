/*
 * A227194  Natural numbers for which all sums Sum_{i=1..u} K(i/n) (with u
 *          ranging from 1 to n) are nonnegative, where K(i/n) is Kronecker
 *          symbol of i and n.
 * https://oeis.org/A227194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth positive integer m such that for all u = 1..m,
 *            Σ_{i=1..u} (i|m) ≥ 0 where (i|m) is the Kronecker symbol. This
 *            is implemented by brute‑force search with a fast Kronecker
 *            symbol
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 64 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

static int kronecker_uint64(uint64_t a, uint64_t n)
{
    /* assume n > 0 */
    /* factor powers of 2 in n */
    int e = 0;
    uint64_t n0 = n;
    while ((n0 & 1ULL) == 0) {
        n0 >>= 1;
        ++e;
    }

    if (e > 0) {
        /* if a is even, symbol is 0 */
        if ((a & 1ULL) == 0)
            return 0;

        /* compute (a|2^e) */
        int a_mod8 = (int)(a & 7ULL);
        int sign2 = 1;
        if (a_mod8 == 3 || a_mod8 == 5)
            sign2 = -1;          /* (-1)^((a^2-1)/8) for odd a */
        if ((e & 1) == 0)      /* e even → factor 1 */
            sign2 = 1;

        if (n0 == 1)           /* only powers of two remain */
            return sign2;

        int jac = jacobi_symbol(a, n0);   /* n0 is odd */
        return sign2 * jac;
    } else { /* n odd */
        return jacobi_symbol(a, n);
    }
}

static bool good(uint64_t m)
{
    int64_t sum = 0;
    for (uint64_t i = 1; i <= m; ++i) {
        sum += kronecker_uint64(i, m);
        if (sum < 0)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    idx_t target = n;
    idx_t count = 0;
    for (idx_t m = 1; ; ++m) {
        if (good((uint64_t)m)) {
            ++count;
            if (count == target)
                return (seq_t)m;
        }
    }
}
