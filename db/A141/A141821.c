/*
 * A141821  Least number k < n and coprime to n such that the largest term of
 *          the continued fraction of k/n is as small as possible.
 * https://oeis.org/A141821
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k < n with gcd(k,n)=1 that minimizes the largest
 *            partial quotient in the continued‑fraction expansion of k/n. If
 *            several k tie, the smallest k is chosen
 * Range:     u8 n <= 327, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

/* Euclidean algorithm for idx_t */
static inline idx_t
gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t
a(idx_t n)
{
    if (n < 2)
        return 0;             /* below offset, any return value is fine */

    idx_t best_k = 0;
    idx_t best_max = (idx_t)-1;   /* largest possible value */

    for (idx_t k = 1; k < n; ++k) {
        if (gcd_idx(k, n) != 1)
            continue;

        idx_t a_val = n;
        idx_t b_val = k;
        idx_t max_q = 0;

        /* Compute continued‑fraction digits; stop early
           if the partial quotient exceeds best_max. */
        while (b_val != 0 && max_q <= best_max) {
            idx_t q = a_val / b_val;
            if (q > max_q) {
                max_q = q;
                if (max_q > best_max)    /* cannot beat current best */
                    break;
            }
            idx_t r = a_val % b_val;
            a_val = b_val;
            b_val = r;
        }

        if (b_val == 0 && max_q <= best_max) {
            if (max_q < best_max) {
                best_max = max_q;
                best_k   = k;
            } else if (max_q == best_max && k < best_k) {
                /* tie – choose smaller k */
                best_k = k;
            }
        }
    }

    return (seq_t)best_k;
}
