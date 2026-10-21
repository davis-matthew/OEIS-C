/*
 * A225179  a(n) = min{2 + c_n + 2(c_1 + c_2 + ... + c_(n-1)) | n = p + q, 1
 *          <= p < q < n, gcd(p, q) = 1, and p/q has continued fraction
 *          expansion [0; c_1, c_2, ...., c_n]}.
 * https://oeis.org/A225179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{ 2 + c_n + 2·(c_1 + … + c_(n-1)) | 1 ≤ p < q < n,
 *            gcd(p,q)=1, and p/q = [0; c_1, c_2, …, c_n] }. The function
 *            enumerates all admissible fractions for the given n, computes
 *            the required value from their continued‑fraction expansions, and
 *            keeps the minimum. For n = 1 and n = 2 the values...
 * Range:     u8 n <= 199, u16 n <= 199, u32 n <= 199, u64 n <= 199,
 *            u128 n <= 199, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Handle the special base cases that have no admissible pairs. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;

    /* Use a 64‑bit integer to accumulate partial‑quotient sums
       and to find the minimum; the final result fits into seq_t. */
    idx_t min_val = (idx_t)-1;   /* largest possible idx_t value */

    /* Enumerate all pairs (p,q) with p+q = n, 1 ≤ p < q < n, gcd(p,q)=1. */
    for (idx_t p = 1; p < n; ++p) {
        idx_t q = n - p;
        if (p >= q) break;                /* ensures p < q < n */
        if (gcd_idx(p, q) != 1) continue;/* skip non‑coprime pairs */

        /* Compute the simple continued fraction of p/q. */
        idx_t num = p;
        idx_t den = q;
        idx_t sum  = 0;
        idx_t last = 0;                   /* last partial quotient (c_n) */

        while (num != 0) {
            idx_t a_part = den / num;     /* current partial quotient */
            sum += a_part;
            last = a_part;
            idx_t r = den % num;
            den = num;
            num = r;
        }

        /* B = 2 + 2·sum – last  (equivalent to 2 + c_n + 2·(sum–c_n)) */
        idx_t B = (idx_t)2 + (idx_t)2 * sum - last;
        if (B < min_val) min_val = B;
    }

    /* The sequence is defined for all n ≥ 1; min_val should have been
       updated for n ≥ 3.  Convert the final minimum back to seq_t. */
    return (seq_t)min_val;
}
