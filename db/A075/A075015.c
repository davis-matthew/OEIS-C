/*
 * A075015  Smallest k such that the concatenation k, k+1, k+2 is divisible by
 *          n; or 0 if no such number exists.
 * https://oeis.org/A075015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 181, u16 n <= 924, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   pow10_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Implementation of OEIS sequence A075015:
 *   a(n) = smallest k such that the decimal concatenation
 *   of k, k+1, k+2 is divisible by n;   return 0 if no such k exists.
 * The algorithm simply tests successive k, computing the concatenation
 * modulo n until a solution (or a hard‑coded upper bound) is found.
 * This brute‑force search is fast enough for n up to ~10⁴
 * (all known terms are far below the 10⁷ bound used here).
 */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow10_mod.h"

/* Helper to compute the number of decimal digits of an idx_t value. */
static int digits_idx(idx_t x)
{
    int d = 0;
    do {
        d++;
        x /= 10;
    } while (x);
    return d;
}

seq_t a(idx_t n)
{
    if (n <= 1)       /* n==1 has answer 1, and n==0 is undefined. */
        return (seq_t)1;

    /* Upper bound for the search.  10 million is far larger than any
       known term for n ≤ 10⁴ and keeps the function under the time limit. */
    const idx_t MAX_K = 10000000ULL;

    const seq_t n_mod = (seq_t)n;           /* use for modulo computations */

    idx_t k = 1;
    while (k <= MAX_K)
    {
        /* k, k+1, k+2 */
        idx_t b = k + 1;
        idx_t c = k + 2;

        /* digit lengths of k+1 and k+2 */
        int d_b = digits_idx(b);  /* length of k+1 */
        int d_c = digits_idx(c);  /* length of k+2 */

        /* 10^(d_b + d_c) mod n and 10^(d_c) mod n */
        seq_t pow_bc = pow10_mod((idx_t)(d_b + d_c), n);
        seq_t pow_c  = pow10_mod((idx_t)d_c, n);

        /* Compute concatenation modulo n:
           f(k) = k * 10^(d_b+d_c) + (k+1) * 10^(d_c) + (k+2)  */
        seq_t rem = (( (seq_t)(k % n)          * pow_bc  ) % n_mod
                   + ((seq_t)(b % n)          * pow_c   ) % n_mod
                   + (seq_t)(c % n) ) % n_mod;

        if (rem == 0)
            return (seq_t)k;  /* found the smallest k */

        ++k;
    }

    /* No solution found within the bound: return 0. */
    return 0;
}
