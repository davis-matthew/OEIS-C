/*
 * A215189  Array t(n,k) of the family ((n+k)/gcd(n+k,4))*(n/gcd(n,4)), read
 *          by antidiagonals.
 * https://oeis.org/A215189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 152, u16 n <= 5150, u32 n <= 5150, u64 n <= 5150,
 *            u128 n <= 5150, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A215189:
   a(n) = ((n'+k')/gcd(n'+k',4)) * (n'/gcd(n',4)),
   where (n',k') are the coordinates in the array t(n,k) obtained
   by reading the array by antidiagonals (sum indices increasing,
   and within an antidiagonal n decreasing from the maximum to 0). */
#include <stdint.h>
#include "../utils/gcd.h"

static seq_t antidiagonal_t(idx_t n, idx_t k) {
    seq_t nseq = (seq_t)n;
    seq_t kseq = (seq_t)k;
    seq_t sum  = nseq + kseq;
    seq_t g1   = gcd(sum, (seq_t)4);
    seq_t part1 = sum / g1;
    seq_t g2   = gcd(nseq, (seq_t)4);
    seq_t part2 = nseq / g2;
    return part1 * part2;
}

seq_t a(idx_t n) {
    /* Find the antidiagonal s such that
       C(s-1) = s*(s+1)/2 <= n < C(s) = (s+1)*(s+2)/2. */
    idx_t low = 0, high = 1;
    /* Grow high until it exceeds the diagonal needed. */
    while (((unsigned __int128)(high + 1) * (high + 2)) / 2 <= n) {
        high <<= 1;              /* double high */
    }
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 cmid = ((unsigned __int128)(mid + 1) * (mid + 2)) / 2;
        if (cmid <= n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t s = low;   /* smallest s with C(s) > n */

    /* Previous cumulative count */
    unsigned __int128 cprev = ((unsigned __int128)s * (s + 1)) / 2;
    idx_t offset = (idx_t)(n - cprev);   /* offset within this antidiagonal, 0‑based */
    idx_t n_in   = s - offset;           /* n coordinate (decreasing order) */
    idx_t k_in   = offset;               /* k coordinate (increasing order) */

    return antidiagonal_t(n_in, k_in);
}
