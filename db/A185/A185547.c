/*
 * A185547  Numbers not of the form floor((1/2)(n+1)^(3/2)); complement of
 *          A185546.
 * https://oeis.org/A185547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th positive integer NOT of the form
 *            floor((1/2)(k+1)^(3/2))
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 753, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* ------------------------------------------------------------------------ */
/*  Integer square‑root for an arbitrary unsigned integer type seq_t.       */
/*  Result is floor(sqrt(x)).  Works for 8,16,32,64,128 bits and for        */
/*  arbitrary‑precision integer classes (C++).                               */
/* ------------------------------------------------------------------------ */
static inline seq_t isqrt_seq(const seq_t x)
{
    seq_t low = 0;
    seq_t high = x;
    seq_t ans = 0;
    while (low <= high)
    {
        seq_t mid = low + (high - low) / 2;
        seq_t sq  = mid * mid;
        if (sq <= x)
        {
            ans = mid;
            low = mid + 1;
        }
        else
        {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

/* ------------------------------------------------------------------------ */
/*  Compute S(k) = floor((k+1)^(3/2) / 2) using only integer arithmetic.    */
/* ------------------------------------------------------------------------ */
static inline seq_t compute_S(idx_t k)
{
    /* a = k + 1 */
    seq_t a  = (seq_t)(k + 1);
    seq_t a2 = a * a;
    seq_t a3 = a2 * a;
    seq_t sq_a3 = isqrt_seq(a3);      /* floor(sqrt((k+1)^3)) */
    return sq_a3 / 2;                  /* integer division truncates */
}

/* ------------------------------------------------------------------------ */
/*  Count how many S(k) are <= x.                                            */
/* ------------------------------------------------------------------------ */
static inline seq_t count_S_upto(const seq_t x)
{
    seq_t count = 0;
    for (idx_t k = 1;; ++k)
    {
        seq_t s = compute_S(k);
        if (s > x) break;
        ++count;
    }
    return count;
}

/* ------------------------------------------------------------------------ */
/*  Main sequence function.  Returns the n-th term of the complement of    */
/*  floor((1/2)(n+1)^(3/2)).                                                */
/* ------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* binary search for minimal X with missing_count(X) >= n */
    idx_t low  = 1;
    /* high is chosen large enough for any n up to approx 10000 */
    idx_t high = 4 * n + 100;

    while (low < high)
    {
        idx_t mid = low + (high - low) / 2;
        seq_t mid_seq = (seq_t)mid;
        seq_t miss = mid_seq - count_S_upto(mid_seq);
        if (miss >= n) high = mid;
        else           low  = mid + 1;
    }
    return (seq_t)low;
}
