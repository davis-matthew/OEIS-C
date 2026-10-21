/*
 * A085912  Group the natural numbers such that the product of the terms of
 *          the n-th group is divisible by n!.
 *          (1),(2),(3,4),(5,6,7,8),(9,10,11,12),(13,14,15,16,17,18),(19,20,21,22,23,24),...
 *          Sequence contains the product pertaining to groups.
 * https://oeis.org/A085912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=0}^{s(n)-1} (L + k) where s(n) = 1 for n=1 or n=2;
 *            otherwise s(n)=n if n is even, and s(n)=n-1 if n is odd. The
 *            starting number L is L = 1 + Σ_{i=1}^{n-1} s(i), which can be
 *            computed in O(1) using the closed form below
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n - 1;                  /* number of completed groups */
    idx_t S = 0;                      /* sum of sizes of groups 1..m */

    if (m != 0) {
        /* Count evens and odds among 1..m */
        idx_t e = m / 2;              /* number of even numbers ≤ m */
        idx_t o = m - e;              /* number of odd numbers ≤ m */

        /* sum of even-sized groups:
           all even k have size k, except k=2 which has size 1.
           Sum of evens = e * (e + 1).  Subtract 1 for k=2. */
        idx_t sum_even_total = e * (e + 1);
        idx_t sum_even = (m >= 2) ? sum_even_total - 1 : 0;

        /* sum of odd-sized groups:
           odd k have size 1 for k=1, otherwise k-1.
           Sum of odds = o^2.  Sum of odd sizes = o^2 - (o-1) = o^2 - o + 1. */
        idx_t sum_odd_size = o * o - (o - 1);

        S = sum_even + sum_odd_size;
    }
    idx_t start = 1 + S;              /* first number in the n-th group */

    /* group size */
    idx_t s = (n <= 2) ? 1 : ((n % 2 == 0) ? n : n - 1);

    seq_t prod = 1;
    for (idx_t i = 0; i < s; ++i) {
        prod *= (seq_t)(start + i);
    }
    return prod;
}
