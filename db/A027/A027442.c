/*
 * A027442  For n odd, >1, not divisible by 3, we can write 3/n = 1/a + 1/b +
 *          1/c with a>b>c>0, a,b,c distinct and odd; sequence gives smallest
 *          a.
 * https://oeis.org/A027442
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A027442
 *
 *  a(n) : for n odd, >1, not divisible by 3, the smallest odd a>0 such that
 *         3/n = 1/a + 1/b + 1/c with a>b>c>0, a,b,c distinct.
 *  Sequence is indexed with offset 5: a(5) is the first value shown in the OEIS entry.
 *
 *  This routine maps the sequence index n to the corresponding integer
 *  m (the odd denominator on the left side) using the simple arithmetic
 *  description of odd numbers ≠ 3 (numbers ≡ 1 or 5 (mod 6)).
 *  It then searches over all admissible odd triples (a,b,c) and returns
 *  the smallest a that satisfies the equation.  The search is bounded by
 *  inequalities that keep the ranges small, giving an overall complexity
 *  of roughly O(m) for each call with m ≲ 3·n.
 *
 *  All arithmetic is performed in the seq_t type, which may be an unsigned
 *  integer of any width or an arbitrary‑precision integer.  No floating‑point
 *  operations are used.  This routine works correctly for any n ≥ 5, and
 *  for n < 5 it returns 0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* values below the offset are undefined; return 0 */
    if (n < 5) return 0;

    /* --- map sequence index to the odd integer m = n > 1, 3∤n --- */
    idx_t t   = n - 5;                /* position in the odd‑not‑divisible‑by‑3 list */
    seq_t m;                          /* the integer on the left side of 3/m */
    if (t & 1U) {
        m = (seq_t)((t >> 1) * 6 + 7);   /* 6k+7 when t is odd */
    } else {
        m = (seq_t)((t >> 1) * 6 + 5);   /* 6k+5 when t is even */
    }

    /* --- search for the smallest admissible a --- */
    seq_t best_a = 0;                 /* 0 means “not found yet” */
    /* lower bound for c : c > m/3, must be odd, at least 3 */
    seq_t c_start = m / 3 + 1;
    if ((c_start & 1U) == 0) c_start++;
    if (c_start < 3) c_start = 3;

    for (seq_t c = c_start; c <= m - 1; c += 2) {
        /* For this c, compute bounds for b */
        seq_t denom_factor = (seq_t)3 * c - m;   /* >0 because c > m/3 */
        if (denom_factor <= 0) continue;         /* safety, should not happen */

        /* b must satisfy: b > m*c/(3c – m)  and  b <= 2*m*c/(3c – m) */
        seq_t b_low_raw = m * c / denom_factor;
        seq_t b_low     = b_low_raw + 1;          /* strict inequality */
        if (b_low <= c) b_low = c + 1;           /* enforce a>b>c */
        if ((b_low & 1U) == 0) b_low++;          /* keep b odd */

        seq_t b_high_raw = (seq_t)2 * m * c / denom_factor;
        seq_t b_high = b_high_raw;
        if (b_high < b_low) continue;           /* no admissible b */

        /* iterate over odd b in [b_low, b_high] */
        for (seq_t b = b_low; b <= b_high; b += 2) {
            /* compute candidate a = m*b*c / (3*b*c - m*(b+c)) */
            seq_t numerator = m * b * c;
            seq_t denom_val = (seq_t)3 * b * c - m * (b + c);
            if (denom_val <= 0) continue;       /* safety */
            if (numerator % denom_val != 0) continue;  /* a not integer */
            seq_t a_candidate = numerator / denom_val;
            if (a_candidate <= b) continue;     /* must satisfy a > b */
            if ((a_candidate & 1U) == 0) continue;   /* a must be odd */

            /* keep the smallest admissible a */
            if (best_a == 0 || a_candidate < best_a) {
                best_a = a_candidate;
            }
        }
    }
    return best_a;
}
