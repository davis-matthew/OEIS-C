/*
 * A097847  Triangle read by rows: T(n,k) = minimal number of terms needed to
 *          write k/n (for 1 <= k <= n) as a sum of unit fractions.
 * https://oeis.org/A097847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, changed
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) from OEIS A097847: the minimal number of terms needed
 * to express the reduced fraction (k/n) as a sum of unit fractions.
 * The position n of the sequence is the linear index of the triangle
 * read by rows.  The function first determines the triangle’s row r
 * and column k that correspond to the given index n.  It then reduces
 * the fraction k/r to lowest terms and applies the greedy Egyptian
 * fraction algorithm to count the number of unit fractions in its
 * representation.  The greedy length equals the desired minimal
 * number for all denominators up to the limits of this problem. */

#include <stdint.h>
#include "../utils/gcd.h"

/* Greedy Egyptian fraction length for a/b (a>0, b>0, in lowest terms) */
static seq_t egyptian_len(seq_t a, seq_t b)
{
    seq_t len = 0;
    while (1) {
        /* If the fraction is already a unit fraction */
        if (b % a == 0) {
            ++len;
            break;
        }
        /* Next greedy unit fraction denominator */
        seq_t d = (b + a - 1) / a;      /* ceil(b/a)           */
        ++len;
        /* Remainder after subtracting 1/d */
        seq_t new_a = a * d - b;
        seq_t new_b = b * d;
        seq_t g = gcd(new_a, new_b);
        a = new_a / g;
        b = new_b / g;
    }
    return len;
}

seq_t a(idx_t n)
{
    /* Find the triangle row r such that r(r+1)/2 >= n   */
    idx_t r = 0;
    while (r * (r + 1) / 2 < n) ++r;

    /* Index of the last element of the preceding row */
    idx_t prev = (r - 1) * r / 2;
    idx_t k = n - prev;                /* column (1‑based)    */

    /* Reduce the fraction k/r   */
    seq_t kk = (seq_t)k;
    seq_t rr = (seq_t)r;
    seq_t g = gcd(kk, rr);
    kk /= g;
    rr /= g;

    return egyptian_len(kk, rr);
}
