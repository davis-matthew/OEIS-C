/*
 * A157230  Number of primitive inequivalent sublattices of square lattice
 *          having mirrors parallel to the diagonals of the unit cell of the
 *          parent lattice of index n.
 * https://oeis.org/A157230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A060594(n) - A019590(n))/2 = 2^(c-1), where for n = 2^e2
 *            · Π p_i^e_i (p_i odd primes) :
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   Let f(e2) = (e2≥2) + (e2≥3)   (0 for e2=0,1; 1 for e2=2; 2 for e2≥3) */
 /*   Let d = number of distinct odd prime factors (i.e. number of p_i).       */
 /*   Then a(n) = 2^( f(e2) + d - 1 ).                                         */
 /*   For n ≤ 2 the sequence is defined to be 0.                               */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 2) return 0;          /* a(1)=a(2)=0 */

    uint64_t nn = (uint64_t) n;
    uint64_t primes[64], exps[64];
    size_t cnt;

    /* Factorize n: primes[i] distinct prime, exps[i] its exponent */
    factor_u64(nn, primes, exps, &cnt);

    uint64_t e2 = 0;       /* exponent of prime 2 */
    unsigned distinct_odd = 0;   /* number of distinct odd primes */

    for (size_t i = 0; i < cnt; ++i) {
        if (primes[i] == 2ULL) {
            e2 = exps[i];
        } else {
            ++distinct_odd;
        }
    }

    /* f(e2) = (e2>=2) + (e2>=3) */
    unsigned f_e2 = (e2 >= 2) + (e2 >= 3);

    /* exponent for the power of two */
    unsigned exp = f_e2 + distinct_odd - 1;

    /* exp ≥ 0 because n > 2 guarantees at least one prime factor (odd or 2) */
    return ((seq_t)1 << exp);
}
