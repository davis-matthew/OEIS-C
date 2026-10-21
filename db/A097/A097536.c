/*
 * A097536  -Sum_{k=1..2*q-1} J(k,q)*J(-4,k)*k/4 as q runs through numbers ==
 *          3 (mod 4), where J(i,j) is the Jacobi symbol.
 * https://oeis.org/A097536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -¼ · Σ_{k=1}^{2q-1} J(k,q)·J(−4,k)·k, where q = 4·n − 1
 *            (so q ≡ 3 (mod 4))
 * Range:     i8 n <= 7, i16 n <= 61, i32 n <= 61, i64 n <= 61, i128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Jacobi symbol (a/n) for odd positive n.
   Returns 0 if n is even or ≤ 0.  For odd n it returns
   1, –1 or 0 if a is not coprime to n. */
static int jacobi_symbol(uint64_t a, uint64_t n)
{
    if (n == 0 || (n & 1ULL) == 0)
        return 0;
    a %= n;
    int result = 1;
    while (a != 0) {
        while ((a & 1ULL) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7ULL;
            if (n_mod8 == 3ULL || n_mod8 == 5ULL)
                result = -result;
        }
        /*swap a and n*/
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3ULL) == 3ULL && (n & 3ULL) == 3ULL)
            result = -result;
        a %= n;
    }
    return (n == 1ULL) ? result : 0;
}

seq_t a(idx_t n)
{
    /* q = 4n – 1  (q is always odd) */
    uint64_t q = (uint64_t)n * 4ULL - 1ULL;
    /* upper bound for k */
    uint64_t maxk = 2ULL * q - 1ULL;

    seq_t sum = 0;

    for (uint64_t k = 1; k <= maxk; ++k) {
        int j1 = jacobi_symbol(k, q);     /* J(k,q)      */
        if (j1 == 0)                      /* zero only contributes nothing */
            continue;

        int j2;                           /* J(−4,k) = J(−1,k) for odd k, 0 for even k */
        if ((k & 1ULL) == 0ULL) {         /* even k */
            j2 = 0;
        } else {                          /* odd k */
            j2 = (( (k - 1ULL) >> 1) & 1ULL) ? -1 : 1;
        }
        if (j2 == 0)
            continue;

        sum += (seq_t)(j1 * j2) * (seq_t)k;
    }

    /* division guaranteed to be integral */
    return -(sum / 4);
}
