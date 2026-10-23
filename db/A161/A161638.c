/*
 * A161638  The largest number of steps in Euclid's algorithm applied to
 *          A157807(n) and A157813(n).
 * https://oeis.org/A161638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   gcd_idx_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = number of steps of Euclid's algorithm applied to the n‑th
 *  reduced fraction in the ordering
 *      1/1, 2/1, 1/2, 1/3, 3/1, 4/1, 3/2, 2/3, 1/4, 1/5, 5/1, …
 *  The fractions are grouped by the sum s = numerator + denominator.
 *  For even s the fractions are listed with ascending numerator;
 *  for odd s with descending numerator.  a(1)=1, a(2)=1, a(3)=2, …  */

/* pre‑compute φ(s) and cumulative sums cum[s] for s up to MAX_S */
#ifndef MAX_S
#define MAX_S 2000      /* enough for n up to about 10 000 */
#endif

#include <stddef.h>      /* defines idx_t, seq_t in the build */
#include "../utils/gcd_idx_t.h"

static seq_t phi[MAX_S + 1];
static idx_t cum[MAX_S + 1];
static int initialized = 0;

/* compute φ(s) for all 0 ≤ s ≤ MAX_S and cum[s] = Σ_{i=2}^s φ(i) */
static void init_cache(void)
{
    if (initialized) return;
    phi[0] = phi[1] = (seq_t)0;
    cum[0] = cum[1] = 0;
    for (idx_t s = 2; s <= MAX_S; ++s) {
        seq_t result = (seq_t)s;
        seq_t temp   = (seq_t)s;
        for (idx_t f = 2; (seq_t)f * (seq_t)f <= temp; ++f) {
            if (temp % f == 0) {
                while (temp % f == 0) temp /= f;
                result -= result / (seq_t)f;
            }
        }
        if (temp > 1) result -= result / temp;
        phi[s] = result;
        cum[s] = cum[s - 1] + (idx_t)result;
    }
    initialized = 1;
}

/* Euclid's algorithm: count number of division steps until remainder 0. */
static inline seq_t euclid_steps(seq_t a, seq_t b)
{
    idx_t steps = 0;
    while (b) {
        ++steps;
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return (seq_t)steps;
}

/* gcd for idx_t, used only to test coprimeness of (i, s) */

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n < 1 */

    init_cache();          /* ensure tables are ready */

    /* find the diagonal s such that cum[s‑1] < n ≤ cum[s] */
    idx_t s = 2;
    while (s <= MAX_S && cum[s] < n) ++s;
    /* for safety: if n exceeds pre‑computed range, extend linearly */
    while (s == MAX_S && cum[s] < n) {
        ++s;
        cum[s] = cum[s - 1] + (idx_t)phi[s]; /* φ(s) defaults to 0 beyond MAX_S */
    }

    idx_t offset = n - cum[s - 1];   /* 1‑based position within this diagonal */
    idx_t target = offset - 1;       /* 0‑based index */

    /* locate fraction with numerator i and denominator s‑i */
    idx_t numerator = 0, denominator = 0;
    idx_t pos = 0;

    if (s % 2 == 0) {                         /* even s: ascending numerator */
        for (idx_t i = 1; i < s; ++i) {
            if (gcd_idx_t(i, s) == 1) {
                if (pos == target) {
                    numerator   = i;
                    denominator = s - i;
                    break;
                }
                ++pos;
            }
        }
    } else {                                   /* odd s: descending numerator */
        for (idx_t i = s - 1; i >= 1; --i) {
            if (gcd_idx_t(i, s) == 1) {
                if (pos == target) {
                    numerator   = i;
                    denominator = s - i;
                    break;
                }
                ++pos;
            }
        }
    }

    return euclid_steps((seq_t)numerator, (seq_t)denominator);
}
