/*
 * A227924  Triangle T(n,k): the number of binary sequences of n zeros and n
 *          ones in which the shortest run is of length k.
 * https://oeis.org/A227924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r , k) where T(r , k) = Count(r , k) - Count(r , k+1).
 *            Count(r , c) counts the binary words of r zeros and r ones all
 *            of whose runs have length at least c. The sequence is indexed by
 *            a(1)=T(1,1), a(2)=T(2,1), a(3)=T(2,2), …
 * Range:     u8 n <= 15, u16 n <= 45, u32 n <= 153, u64 n <= 561,
 *            u128 n <= 1275, big any n
 * Helpers:   binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Utility: binomial coefficients (idx_t parameters).  */
#include "../utils/binomial.h"

/*  Utility: multiplication of seq_t values.  */
#include "../utils/seq_mul.h"

static seq_t ncomp(idx_t total, idx_t parts, idx_t minlen)
{
    /* number of weak compositions of "total" into "parts" parts,
       each part >= minlen.  */
    if (total < parts * minlen)
        return 0;
    /* after giving minlen to each part, distribute the remainder.  */
    idx_t rem   = total - parts * minlen;
    idx_t n     = rem + parts - 1;   /* numerator of binomial */
    idx_t k     = parts - 1;         /* denominator of binomial */
    return binomial(n, k);
}

/*  Count(r , minlen): number of binary sequences with r zeros and r ones,
    all runs of zeros AND ones having length at least minlen.  */
static seq_t Count(idx_t r, idx_t minlen)
{
    if (minlen > r)
        return 0;
    seq_t total = 0;
    idx_t max_t = (r * 2) / minlen;   /* maximum possible number of runs */
    for (idx_t t = 2; t <= max_t; ++t) {
        /* consider both starting symbols */
        for (int start = 0; start < 2; ++start) {
            idx_t z_runs, o_runs;
            if (start == 0) {              /* start with 0 */
                z_runs = (t + 1) / 2;
                o_runs = t / 2;
            } else {                        /* start with 1 */
                z_runs = t / 2;
                o_runs = (t + 1) / 2;
            }
            if (z_runs == 0 || o_runs == 0)
                continue;
            seq_t w0 = ncomp(r, z_runs, minlen);
            seq_t w1 = ncomp(r, o_runs, minlen);
            seq_t prod = seq_mul(w0, w1);
            total += prod;
        }
    }
    return total;
}

seq_t a(idx_t n)
{
    /* Find r such that r*(r-1)/2 < n <= r*(r+1)/2.  */
    idx_t low  = 1;
    idx_t high = 1;
    /* Inflate high until it covers the index. */
    while (high * (high + 1) / 2 < n) {
        high *= 2;
    }
    /* Binary search for the smallest r with r*(r+1)/2 >= n. */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;
    idx_t prev = r * (r - 1) / 2;
    idx_t k = n - prev;                 /* 1 <= k <= r */

    /* Result is difference of counts with minimum run lengths k and k+1. */
    seq_t c_k   = Count(r, k);
    seq_t c_k1  = Count(r, k + 1);
    return c_k - c_k1;
}
