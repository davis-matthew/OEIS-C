/*
 * A188425  Position of 2^n in A051037 (5-smooth numbers).
 * https://oeis.org/A188425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(a,b,c)∈ℕ³ | 2^a·3^b·5^c ≤ 2^n}
 * Range:     u8 n <= 15, u16 n <= 110, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* i.e. a(n) is the number of 5‑smooth (Hamming) numbers not exceeding 2^n */

#include <math.h>

#define LOG2_3 1.584962500721156191
#define LOG2_5 2.321928094887362347

seq_t a(idx_t n)
{
    /* Count the triples (b,c) and sum floor(t)+1 where t = n – b*log2(3) – c*log2(5) */
    unsigned long long count = 0ULL;          /* will hold the final result */
    const double L3 = LOG2_3;
    const double L5 = LOG2_5;

    /* maximum b with b*L3 ≤ n */
    int max_b = (int)((double)n / L3 + 1e-12);
    for (int b = 0; b <= max_b; ++b) {
        double rem_b = (double)n - b * L3;
        if (rem_b < 0.0) break;

        /* maximum c with b*L3 + c*L5 ≤ n */
        int max_c = (int)(rem_b / L5 + 1e-12);

        double t = rem_b;                 /* t = n - b*L3 - c*L5 for current c */
        for (int c = 0; c <= max_c; ++c) {
            if (t < 0.0) break;           /* safety guard for rounding */
            count += (unsigned long long)(floor(t)) + 1ULL;
            t -= L5;
        }
    }

    return (seq_t)count;
}
