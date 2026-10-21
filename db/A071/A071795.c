/*
 * A071795  Numerators of a(n+1) = Sum_{k=1..n} a'(n/k), a(1)=1, where
 *          a'(x)=a(x) if x integer and is linearly interpolated otherwise.
 * https://oeis.org/A071795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) is the numerator of the rational number a(n) =
 *            Σ_{k=1}^{n-1} a'( (n-1)/k ), where a'(x) = a(x) if x is integer,
 *            otherwise a'(x) is the linear interpolation between a(⌊x⌋) and
 *            a(⌈x⌉). a(1) = 1. The function returns the numerator of the
 *            reduced fraction a(n)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 29, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The base types (idx_t and seq_t) are supplied by the build.  This
   implementation caches all computed a(n) values (numerator and
   denominator) to allow out‑of‑order calls. */
#include <stdlib.h>
#include "../utils/gcd.h"

/* Cached numerators and denominators of a(n) as reduced fractions. */
static seq_t *num = NULL;   /* numerator of a(n) */
static seq_t *den = NULL;   /* denominator of a(n) */
static size_t cap = 0;      /* allocated capacity (>= current size) */
static idx_t computed = 0;  /* highest n for which a(n) has been computed */

/* Ensure that the arrays are large enough to hold indices up to N. */
static void ensure_capacity(idx_t N) {
    if (N + 1 <= cap) return;
    size_t new_cap = cap ? (cap << 1) : 256;
    while (new_cap < (size_t)(N + 1)) new_cap <<= 1;
    num = (seq_t *)realloc(num, (new_cap + 1) * sizeof(seq_t));
    den = (seq_t *)realloc(den, (new_cap + 1) * sizeof(seq_t));
    cap = new_cap;
}

/* Compute all a(n) values up to N (inclusive). */
static void compute_up_to(idx_t N) {
    if (N <= computed) return;
    ensure_capacity(N);

    /* Initialise a(1) if first time. */
    if (computed == 0) {
        num[1] = (seq_t)1;
        den[1] = (seq_t)1;
        computed = 1;
    }

    for (idx_t n = computed + 1; n <= N; ++n) {
        /* Compute a(n) = Σ_{k=1}^{n-1} a'((n-1)/k). */
        idx_t m = n - 1;
        seq_t sum_num = 0;
        seq_t sum_den = 1;

        for (idx_t k = 1; k <= m; ++k) {
            idx_t div = m / k;          /* floor((n-1)/k) */
            idx_t rem = m % k;          /* remainder (0 <= rem < k) */
            seq_t val_num, val_den;

            if (rem == 0) {        /* integer argument */
                val_num = num[div];
                val_den = den[div];
            } else {               /* linear interpolation */
                idx_t low  = div;          /* floor value */
                idx_t high = div + 1;      /* ceil value, safe */

                seq_t r      = (seq_t)rem;  /* remainder as seq_t */
                seq_t k_seq  = (seq_t)k;    /* k as seq_t */

                seq_t num_low  = num[low];
                seq_t den_low  = den[low];
                seq_t num_high = num[high];
                seq_t den_high = den[high];

                /* part1 = (k - r)/k * a(low)   --> (k - r)*num_low*den_high
                 * part2 = r/k * a(high)      --> r*num_high*den_low
                 * denominator = k*den_low*den_high
                 */
                seq_t part1 = num_low * den_high;
                part1 *= (k_seq - r);
                seq_t part2 = num_high * den_low;
                part2 *= r;

                val_num = part1 + part2;
                val_den = k_seq * den_low * den_high;

                /* reduce the interpolation fraction */
                seq_t g = gcd(val_num, val_den);
                val_num /= g;
                val_den /= g;
            }

            /* Add val to the running sum: sum = sum + val */
            seq_t new_num = sum_num * val_den + val_num * sum_den;
            seq_t new_den = sum_den * val_den;
            seq_t g2 = gcd(new_num, new_den);
            sum_num = new_num / g2;
            sum_den = new_den / g2;
        }

        num[n] = sum_num;
        den[n] = sum_den;
    }

    computed = N;
}

/* Public function required by the build. */
seq_t a(idx_t n) {
    if (n == 0) return 0;                /* offset 1; behaviour for n < 1 not required */
    compute_up_to(n);
    return num[n];                       /* numerator of the reduced fraction a(n) */
}
