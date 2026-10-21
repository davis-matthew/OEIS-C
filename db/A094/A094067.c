/*
 * A094067  Triangle read by rows: T(n,k) is the number of permutations p of
 *          [n] in which the length of the longest initial segment avoiding
 *          the 123-, the 132- and the 321-pattern is equal to k.
 * https://oeis.org/A094067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th element in the triangle T read rowwise, where
 *            T(n,k) = n! * ((k+1)Fib(k+1)-Fib(k+2))/(k+1)! for 1≤k<n and
 *            T(n,n)=Fib(n+1). The row sum of T is n!, and the diagonal yields
 *            Fibonacci numbers. This routine maps the index n to the correct
 *            (row,column) pair and evaluates T(n,k)
 * Range:     u8 n <= 16, u16 n <= 37, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   factorial, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/fib.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* outside official offset */
    /* Find the row i such that i*(i+1)/2 >= n */
    idx_t i = 0, total = 0;
    while (total < n) {
        ++i;
        total += i;
    }
    /* Column index j (1‑based) */
    idx_t j = n - (total - i);

    /* Diagonal: T(n,n) = Fib(n+1) */
    if (j == i) {
        return fib(i + 1);
    }

    /* Off‑diagonal: T(n,k) = n! * ((k+1)Fib(k+1)-Fib(k+2))/(k+1)! */
    seq_t numerator = (seq_t)(j + 1) * fib(j + 1) - fib(j + 2);
    seq_t fac_n   = factorial(i);
    seq_t fac_k1  = factorial(j + 1);
    seq_t coeff   = fac_n / fac_k1;   /* exact division */
    return coeff * numerator;
}
