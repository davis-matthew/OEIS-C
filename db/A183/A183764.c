/*
 * A183764  Number of 2 X 2 singular 0..n matrices with a(1,1) <= a(1,2) <=
 *          a(2,1) <= a(2,2).
 * https://oeis.org/A183764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 2×2 singular matrices with entries 0..n
 *            satisfying a11 ≤ a12 ≤ a21 ≤ a22. The count is computed as: a(n)
 *            = binom(n+2, 2) // case a11 = 0 + Σ_{1 ≤ a ≤ b ≤ n} max(0,
 *            floor(n / (b/g)) - ceil(b / (a/g)) + 1), where g = gcd(a,b) and
 *            a/g, b/g are coprime. The summation counts, for each...
 * Range:     u8 n <= 14, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Euclidean gcd for unsigned integer type idx_t */
static inline idx_t gcd_idx(idx_t a, idx_t b) {
    while (b) {
        idx_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

seq_t a(idx_t n) {
    seq_t count = 0;

    /* Case a11 = 0 : only b12 = 0 gives singularity.
       Count non‑decreasing pairs (c,d) from 0..n. */
    count += ((seq_t)n + 2) * ((seq_t)n + 1) / 2;

    for (idx_t a = 1; a <= n; ++a) {
        for (idx_t b = a; b <= n; ++b) {
            idx_t g = gcd_idx(a, b);
            idx_t a_prime = a / g;
            idx_t b_prime = b / g;

            /* t corresponds to c = a'*t and d = b'*t.
               Constraints:  c ≥ b  and  d ≤ n. */
            idx_t t_min = (b + a_prime - 1) / a_prime;  /* ceil(b / a') */
            idx_t t_max = n / b_prime;                 /* floor(n / b') */

            if (t_max >= t_min) {
                count += (seq_t)(t_max - t_min + 1);
            }
        }
    }
    return count;
}
