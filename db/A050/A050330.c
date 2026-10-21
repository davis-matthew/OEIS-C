/*
 * A050330  Number of factorizations of n into numbers with an odd number of
 *          prime factors.
 * https://oeis.org/A050330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2519, u16 n <= 2519, u32 n <= 2519, u64 n <= 2519,
 *            u128 n <= 2519, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recursively counts unordered factorizations of n into factors
   whose big-Omega (total prime multiplicity) is odd.  */
/* The function a(n) is defined for n ≥ 1. */

#include <stdint.h>
#include <stddef.h>

/* Forward declaration of the recursive helper */
static seq_t rec_factor(uint64_t n, uint64_t min_factor);

/* Simple trial‑division prime factorisation of n.
   The caller supplies two arrays of length at least 64 and a
   pointer to a size_t in which the number of distinct prime factors
   is returned. */
static void factor_uint64(uint64_t n, uint64_t *pr, int *exp, size_t *cnt)
{
    size_t c = 0;
    if (n <= 1) {
        *cnt = 0;
        return;
    }
    /* Extract factor 2 first */
    if ((n & 1ULL) == 0) {
        int e = 0;
        while ((n & 1ULL) == 0) {
            n >>= 1;
            ++e;
        }
        pr[c] = 2ULL;
        exp[c] = e;
        ++c;
    }
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            pr[c] = p;
            exp[c] = e;
            ++c;
        }
    }
    if (n > 1) {
        pr[c] = n;
        exp[c] = 1;
        ++c;
    }
    *cnt = c;
}

/* Helper that enumerates all divisors of n using its prime factorisation.
   For each divisor d >= min_factor with odd Ω(d) the recursion
   rec_factor(n/d, d) is invoked. */
static void dfs_divs(size_t pos, uint64_t cur, int omega, uint64_t min_factor,
                     uint64_t n, uint64_t *pr, int *exp, size_t pcnt,
                     seq_t *total)
{
    if (pos == pcnt) {
        if (cur >= min_factor && cur > 1 && (omega & 1)) {
            *total += rec_factor(n / cur, cur);
        }
        return;
    }
    uint64_t mult = 1;
    for (int e = 0; e <= exp[pos]; ++e) {
        dfs_divs(pos + 1, cur * mult, omega + e, min_factor, n,
                 pr, exp, pcnt, total);
        mult *= pr[pos];
    }
}

/* Recursive count of factorizations.  n is the remaining product
   and min_factor enforces non‑decreasing order of factors. */
static seq_t rec_factor(uint64_t n, uint64_t min_factor)
{
    if (n == 1) return 1;

    uint64_t pr[64];
    int exp[64];
    size_t pcnt;
    factor_uint64(n, pr, exp, &pcnt);

    seq_t total = 0;
    dfs_divs(0, 1, 0, min_factor, n, pr, exp, pcnt, &total);
    return total;
}

/* Public API expected by the tests */
seq_t a(idx_t n)
{
    return rec_factor((uint64_t)n, 2ULL);
}
