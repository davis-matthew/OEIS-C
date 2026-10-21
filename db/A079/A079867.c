/*
 * A079867  a(1)=1 and for n>1: floor(n^(1/Omega(n)))^Omega(n), where Omega(n)
 *          is the total number of prime factors of n (A001222).
 * https://oeis.org/A079867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n^(1/Omega(n)))^Omega(n)
 * Range:     u8 n <= 23, u16 n <= 59, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, bigomega, seq_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Omega(n) = number of prime factors of n counted with multiplicity (A001222) */

#include "../utils/bigomega.h"
#include "../utils/big_pow.h"
#include "../utils/seq_cmp.h"

/* Find floor of N^(1/exp) for integer exp >= 1.
   Returns the largest seq_t x such that x^exp <= N. */
static seq_t integer_nth_root(seq_t N, seq_t exp)
{
    /* For exp == 1, the root is N itself. */
    if (exp <= 1)
        return N;

    seq_t low  = 1;
    seq_t high = N;              /* high is >= the desired root */

    while (low < high) {
        /* mid = ceil((low+high)/2) to avoid infinite loop when low+1 == high */
        seq_t mid = (low + high + 1) / 2;
        seq_t mid_pow = big_pow(mid, exp);

        /* Compare mid_pow with N */
        if (seq_cmp(&mid_pow, &N) <= 0) {
            low = mid;                /* mid is feasible */
        } else {
            high = mid - 1;           /* mid is too large */
        }
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;

    /* Compute Omega(n) as a seq_t; it will fit in seq_t for n up to 10⁶ and beyond. */
    seq_t omega = bigomega(n);
    /* Compute floor(n^(1/omega)) */
    seq_t root = integer_nth_root((seq_t)n, omega);
    /* Return root^omega (which is <= n) */
    return big_pow(root, omega);
}
