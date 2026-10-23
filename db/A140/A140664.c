/*
 * A140664  a(n) = A014963(n)*mobius(n).
 * https://oeis.org/A140664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A014963(n) * mobius(n) A014963(n) = n if n is prime 1 if
 *            n is squarefree with ≥2 distinct primes arbitrary otherwise
 *            (multiplied by mu gives 0)
 * Range:     i8 n <= 130, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   count_distinct_primes, mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mobius.h"
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    seq_t mu = mobius(n);
    if (mu == 0)
        return 0;
    if (mu == 1)
        return 1;
    /* mu == -1 */
    int d = count_distinct_primes(n);
    if (d == 1)
        return -(seq_t)n;
    else
        return -1;
}
