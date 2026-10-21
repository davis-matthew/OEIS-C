/*
 * A212014  Total number of states of the first n subshells of the nuclear
 *          shell model in which the subshells are ordered by energy level in
 *          increasing order.
 * https://oeis.org/A212014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 2 * (S_{k-1} + (offset+1)*k - offset*(offset+1)/2) where
 *            k = ceil( (sqrt(8*n+1)-1)/2 ) (smallest k with k*(k+1)/2 >= n)
 *            offset = n - (k-1)*k/2 - 1 S_{p} = p*(p+1)*(p+2)/6
 *            (sum_{m=1}^{p} m(m+1)/2) This gives a(1)=2, a(2)=6, a(3)=8, … as
 *            in OEIS A212014
 * Range:     u8 n <= 36, u16 n <= 1657, u32 n <= 8687, u64 n <= 8687,
 *            u128 n <= 8687, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Find the minimal k such that k*(k+1)/2 >= n.
   Works for all n in [1, UINT64_MAX].  */
static idx_t minimal_k(idx_t n)
{
    idx_t lo = 0, hi = 1;
    /* Double hi until it satisfies the inequality.  */
    while ((unsigned __int128)hi * (hi + 1) / 2 < n)
        hi <<= 1;

    /* Binary search between lo+1 and hi.  */
    while (lo + 1 < hi) {
        idx_t mid = (lo + hi) >> 1;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            hi = mid;
        else
            lo = mid;
    }
    return hi;   /* hi is the minimal k with k*(k+1)/2 >= n.  */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety – the sequence is defined for n ≥ 1 */

    idx_t k = minimal_k(n);                     /* k ≥ 1 */
    /* T_{k-1} = (k-1)*k/2  */
    idx_t t_prev = (unsigned __int128)(k - 1) * k / 2;

    /* offset = n - T_{k-1} - 1  (0‑based within the k‑th group) */
    idx_t offset = n - t_prev - 1;
    idx_t offset_plus_one = offset + 1;

    /* S_{k-1} = (k-1)*k*(k+1)/6  */
    seq_t S = (seq_t)(((unsigned __int128)(k - 1) * k * (k + 1)) / 6);

    /* partial = (offset+1)*k - offset*(offset+1)/2  */
    seq_t partial = (seq_t)((unsigned __int128)offset_plus_one * k -
                            (unsigned __int128)offset * offset_plus_one / 2);

    seq_t b = S + partial;    /* b(n) */
    return 2 * b;             /* a(n) = 2 * b(n) */
}
