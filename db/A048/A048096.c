/*
 * A048096  Number of nonempty subsets of {1,2,...,n} in which exactly 1/4 of
 *          the elements are <= sqrt(n).
 * https://oeis.org/A048096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Σₖ C(⌊√n⌋, k) · C(n-⌊√n⌋, 3k)
 *   summed over all k that satisfy
 *     1 ≤ k ≤ ⌊√n⌋,
 *     4k ≤ n,
 *     3k ≤ n-⌊√n⌋
 *
 *   (empty subset is excluded by k≥1)
 */

#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Integer square‑root (floor) for 0 ≤ n < 2⁶⁴                           */
/* ------------------------------------------------------------------ */
static inline idx_t int_sqrt(idx_t n) {
    idx_t low = 0, high = (n < ((idx_t)1 << 32)) ? n : ((idx_t)1 << 32);
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0 || mid > n / mid) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return high;
}

/* ------------------------------------------------------------------ */
/*  Greatest common divisor for seq_t (works for arbitrary precision) */
/* ------------------------------------------------------------------ */
static inline seq_t seq_gcd(seq_t a, seq_t b) {
    while (b != 0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* ------------------------------------------------------------------ */
/*  Multiplicative binomial coefficient
 *   C(n, k) for 0 ≤ k ≤ n  (returns 0 if k>n)
 * ------------------------------------------------------------------ */
static inline seq_t binom(idx_t n, idx_t k) {
    if (k > n) return (seq_t)0;
    if (k == 0 || k == n) return (seq_t)1;
    if (k > n - k) k = n - k;     /* use symmetry */

    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t numerator   = n - k + i;   /* (n-k+i) */
        seq_t denominator = i;           /* i */

        /* cancel common factors with the current result */
        seq_t g = seq_gcd(result, denominator);
        result /= g;
        denominator /= g;

        /* cancel common factors between numerator and denominator */
        g = seq_gcd(numerator, denominator);
        numerator /= g;
        denominator /= g;

        /* multiply and divide (division is exact) */
        result *= numerator;
        result /= denominator;
    }
    return result;
}

/* ------------------------------------------------------------------ */
/*  Main routine for OEIS A048096                                       */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    idx_t m = int_sqrt(n);          /* floor(sqrt(n)) */
    idx_t n_minus_m = n - m;

    /* Determine the largest k that satisfies the constraints */
    idx_t tmax = m;
    if (n / 4            < tmax) tmax = n / 4;
    if ((n_minus_m) / 3 < tmax) tmax = n_minus_m / 3;

    seq_t sum = 0;
    for (idx_t k = 1; k <= tmax; ++k) {
        seq_t left  = binom(m, k);
        seq_t right = binom(n_minus_m, 3 * k);
        sum += left * right;
    }
    return sum;
}
