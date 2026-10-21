/*
 * A072509  Number of Fibonacci numbers F(k) <= 10^n which end in 1.
 * https://oeis.org/A072509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A072509: a(n) = number of Fibonacci numbers F(k) ≤ 10ⁿ that end in 1.
   The algorithm uses a fast approximation for the largest index k such that
   F(k) ≤ 10ⁿ.  For small n (≤10) it falls back to a direct iteration that
   works with 64‑bit values. */
#include <math.h>

static const idx_t residues[8] = {1, 2, 8, 19, 22, 28, 41, 59};

seq_t a(idx_t n)
{
    /* ----- small n: compute directly with 64‑bit arithmetic ----- */
    if (n <= 10) {
        seq_t limit = 1;
        for (idx_t i = 0; i < n; ++i)
            limit *= (seq_t)10;

        seq_t f0 = 0, f1 = 1, count = 0;
        if (f1 <= limit && (f1 % (seq_t)10) == 1)
            ++count;

        while (1) {
            seq_t f_next = f0 + f1;
            if (f_next > limit)
                break;
            if ((f_next % (seq_t)10) == 1)
                ++count;
            f0 = f1;
            f1 = f_next;
        }

        return count;
    }

    /* ----- larger n: use logarithmic estimate and binary search ----- */
    long double log10_phi = log10l((1.0L + sqrtl(5.0L)) / 2.0L);
    long double log10_sqrt5 = log10l(sqrtl(5.0L));

    /* Upper bound for k: n / log10(phi) + margin */
    long double approx = (long double)n / log10_phi;
    idx_t high = (idx_t)approx + 20;      /* safety margin */
    idx_t low = 0;

    while (low < high) {
        idx_t mid = (low + high + 1) / 2;
        long double logF = mid * log10_phi - log10_sqrt5;
        if (logF <= (long double)n + 1e-12L)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t k_max = low;   /* largest index with F(k) ≤ 10ⁿ */

    /* Count indices congruent to the 8 residues modulo 60 */
    seq_t count = (seq_t)(k_max / 60) * 8;
    idx_t rem = k_max % 60;
    for (int i = 0; i < 8; ++i) {
        if (residues[i] <= rem)
            ++count;
        else
            break;
    }

    return count;
}
