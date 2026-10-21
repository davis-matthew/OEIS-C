/*
 * A212013  Triangle read by rows: total number of pairs of states of the
 *          first n subshells of the nuclear shell model in which the
 *          subshells are ordered by energy level in increasing order.
 * https://oeis.org/A212013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(S,3) + n + (n - binomial(S,2)) *
 *            (S*(S+3)-2*n-2)/4 where S = floor(R/2) + (R mod 2) and R =
 *            floor(sqrt(8*n+1))
 * Range:     u8 n <= 58, u16 n <= 2638, u32 n <= 9926, u64 n <= 9926,
 *            u128 n <= 9926, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for a 64‑bit value v.  Returns floor(sqrt(v)). */
static idx_t int_sqrt(idx_t v)
{
    idx_t lo = 0, hi = v, mid, ans = 0;
    while (lo <= hi) {
        mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * (unsigned __int128)mid;
        if (sq <= v) {
            ans = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return ans;
}

/* Binomial coefficient C(s,2) using seq_t arithmetic. */
static seq_t binom2(seq_t s)
{
    return (s * (s - 1)) / 2;
}

/* Binomial coefficient C(s,3) using seq_t arithmetic. */
static seq_t binom3(seq_t s)
{
    return (s * (s - 1) * (s - 2)) / 6;
}

/* OEIS A212013: total number of pairs of states of the first n subshells. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1, but safety for out‑of‑range */

    /* Compute R = floor(sqrt(8*n+1)) */
    unsigned __int128 tmp = (unsigned __int128)8 * n + 1;
    idx_t R = int_sqrt(tmp);

    /* Compute S = floor(R/2) + R mod 2 */
    idx_t S = (R >> 1) + (R & 1);

    /* Convert S and n to seq_t for arithmetic */
    seq_t s      = (seq_t)S;
    seq_t n_seq  = (seq_t)n;

    /* B2 = C(S,2), B3 = C(S,3) */
    seq_t B2 = binom2(s);
    seq_t B3 = binom3(s);

    /* temp = n - B2 */
    seq_t temp = n_seq - B2;

    /* part = S*(S+3) - 2*n - 2 */
    seq_t part = s * (s + 3) - (n_seq * 2) - 2;

    /* mult_div4 = temp * part / 4 */
    seq_t mult_div4 = (temp * part) / 4;

    /* a(n) = B3 + n + mult_div4 */
    return B3 + n_seq + mult_div4;
}
