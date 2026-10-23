/*
 * A186760  Number of cycles that are either nonincreasing or of length 1 in
 *          all permutations of {1,2,...,n}. A cycle (b(1), b(2), ...) is said
 *          to be increasing if, when written with its smallest element in the
 *          first position, it satisfies b(1)<b(2)<b(3)<... .
 * https://oeis.org/A186760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! + Σ_{L=2..n} (n!/L - n!/L!)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This follows from: a(n) = n! + Σ_{L=2..n} C(n,L) · ((L‑1)! – 1) · (n-L)!  */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(0) has no cycles */
    if (n == 0) return (seq_t)0;

    /* compute n! */
    seq_t fact = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        fact *= (seq_t)i;

    seq_t res = fact;            /* the length‑1 cycles count */
    seq_t ratio = (seq_t)1;      /* product of numbers from L+1 to n  */

    for (idx_t L = n; L >= 2; --L) {
        seq_t part  = fact / (seq_t)L;   /* n! / L      */
        seq_t term  = part - ratio;      /* (n!/L) - (n!/L!) */
        res += term;
        ratio *= (seq_t)L;                /* update product for next L */
    }

    return res;
}
