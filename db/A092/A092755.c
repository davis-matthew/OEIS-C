/*
 * A092755  Partial sums of A000195 (floor(log(n))).
 * https://oeis.org/A092755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} ⌊log(k)⌋ (natural logarithm, base e)
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Pre‑computed lower bounds ⌈e^t⌉ for successive t.
       For every t,   floor(log(k)) = t  ⇔  ⌈e^t⌉ ≤ k < ⌈e^{t+1}⌉  */
    static const idx_t thresholds[] = {
        1ULL,    /* ⌈e^0⌉ */
        3ULL,    /* ⌈e^1⌉ */
        8ULL,    /* ⌈e^2⌉ */
        21ULL,   /* ⌈e^3⌉ */
        55ULL,   /* ⌈e^4⌉ */
        149ULL,  /* ⌈e^5⌉ */
        404ULL,  /* ⌈e^6⌉ */
        1097ULL, /* ⌈e^7⌉ */
        2981ULL, /* ⌈e^8⌉ */
        8104ULL, /* ⌈e^9⌉ */
        22027ULL /* ⌈e^10⌉, added for safety up to n≈10 000 */
    };
    const size_t nb = sizeof(thresholds) / sizeof(thresholds[0]);

    /* Find the largest t such that thresholds[t] ≤ n. */
    idx_t t = 0;
    while (t + 1 < (idx_t)nb && thresholds[t + 1] <= n) {
        ++t;
    }

    seq_t sum = 0;

    /* Full intervals [thresholds[tp], thresholds[tp+1]-1] for tp < t. */
    for (idx_t tp = 0; tp < t; ++tp) {
        seq_t cnt = (seq_t)(thresholds[tp + 1] - thresholds[tp]); /* how many k with floor(log)=tp */
        sum += (seq_t)tp * cnt;
    }

    /* Last (possibly partial) interval [thresholds[t], n]. */
    seq_t cnt_last = (seq_t)( (idx_t)n - thresholds[t] + 1);
    sum += (seq_t)t * cnt_last;

    return sum;
}
