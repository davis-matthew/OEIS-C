/*
 * A060764  Number of divisors of n which are not also differences between
 *          consecutive divisors (ordered by increasing magnitude) of n.
 * https://oeis.org/A060764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors of n that are not equal to any
 *            difference of consecutive divisors of n (divisors listed in
 *            increasing order)
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9977 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Binary search for a 64‑bit value in a sorted array.  Return 1 if
   found, 0 otherwise. */
static int binary_search_uint64(const uint64_t *arr, size_t len, uint64_t target)
{
    size_t low = 0, high = len;
    while (low < high) {
        size_t mid = (low + high) >> 1;
        if (arr[mid] < target)
            low = mid + 1;
        else if (arr[mid] > target)
            high = mid;
        else
            return 1;
    }
    return 0;
}

/* Factorise `n` into prime factors stored in `pr` and exponents in
   `exp`.  `cnt` will contain the number of distinct prime factors.
   All arrays must be large enough (we use 32 entries).  The function
   returns the product of (exponent+1) = number of divisors. */
static uint64_t factor_and_divisor_count(uint64_t n, uint64_t *pr, uint64_t *exp, size_t *cnt)
{
    *cnt = 0;
    uint64_t tau = 1;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            pr[(*cnt)] = p;
            exp[(*cnt)] = e;
            tau *= (e + 1);
            (*cnt)++;
        }
    }
    if (n > 1) {                     /* remaining prime factor */
        pr[(*cnt)] = n;
        exp[(*cnt)] = 1;
        tau *= 2;
        (*cnt)++;
    }
    return tau;
}

/* Generate all divisors given the prime factorisation.  The array
   `divs` must have space for at least `tau` elements (tau = number of
   divisors).  `cnt` is the number of distinct prime factors. */
static void generate_divs(uint64_t cur, size_t idx, uint64_t *divs, size_t *pos,
                          const uint64_t *pr, const uint64_t *exp, size_t cnt)
{
    if (idx == cnt) {
        divs[*pos] = cur;
        (*pos)++;
        return;
    }
    uint64_t p = pr[idx];
    uint64_t e = exp[idx];
    uint64_t value = cur;
    for (uint64_t k = 0; k <= e; ++k) {
        generate_divs(value, idx + 1, divs, pos, pr, exp, cnt);
        value *= p;
    }
}

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Comparator for qsort on uint64_t arrays. */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t av = *(const uint64_t *)a;
    uint64_t bv = *(const uint64_t *)b;
    return (av < bv) ? -1 : (av > bv);
}

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* undefined for n=0, return 0 safely */

    uint64_t num = (uint64_t)n;      /* work in native uint64_t */

    /* Factorisation */
    uint64_t pr[32];
    uint64_t exp[32];
    size_t cnt = 0;
    uint64_t tau = factor_and_divisor_count(num, pr, exp, &cnt);

    /* Allocate space for all divisors */
    uint64_t *divs = (uint64_t *)malloc(tau * sizeof(uint64_t));
    if (!divs) return (seq_t)0;       /* allocation failure */

    size_t pos = 0;
    generate_divs(1ULL, 0, divs, &pos, pr, exp, cnt);
    /* pos should equal tau */
    qsort(divs, tau, sizeof(uint64_t), cmp_uint64);

    /* Compute differences between consecutive divisors */
    size_t diff_count = (tau > 0) ? tau - 1 : 0;
    uint64_t *diffs = NULL;
    if (diff_count > 0) {
        diffs = (uint64_t *)malloc(diff_count * sizeof(uint64_t));
        if (!diffs) { free(divs); return (seq_t)0; }
        for (size_t i = 0; i < diff_count; ++i)
            diffs[i] = divs[i + 1] - divs[i];
        qsort(diffs, diff_count, sizeof(uint64_t), cmp_uint64);
    }

    /* Count divisors that are not equal to any difference */
    size_t not_diff = 0;
    for (size_t i = 0; i < tau; ++i) {
        if (!binary_search_uint64(diffs, diff_count, divs[i]))
            ++not_diff;
    }

    /* Clean up */
    free(divs);
    free(diffs);

    return (seq_t)not_diff;
}
