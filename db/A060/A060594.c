/*
 * A060594  Number of solutions to x^2 == 1 (mod n), that is, square roots of
 *          unity modulo n.
 * https://oeis.org/A060594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = a(2^e) * 2^m where m is the number of distinct odd prime
 *            factors of n and a(2^e) = 1 (e=0 or 1), 2 (e=2), 4 (e>2). The
 *            sequence is multiplicative, with a(1)=1
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   big_pow, count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include helpers for counting distinct primes and computing big powers */
#include <stddef.h>
#include "../utils/count_distinct_primes.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* Exponent of 2 in n */
    idx_t m = n;
    idx_t v2 = 0;
    while ((m & 1ULL) == 0) {
        v2++;
        m >>= 1;
    }

    /* Number of distinct odd prime factors */
    idx_t odd_cnt = count_distinct_primes(n);
    if ((n & 1ULL) == 0)          /* even: exclude the factor 2 */
        odd_cnt--;

    /* 2^odd_cnt, works for any seq_t (uses arbitrary precision if needed) */
    seq_t res = big_pow((seq_t)2, (seq_t)odd_cnt);

    /* Multiply by the appropriate factor depending on the power of 2 */
    seq_t mult;
    if (v2 <= 1)
        mult = (seq_t)1;
    else if (v2 == 2)
        mult = (seq_t)2;
    else
        mult = (seq_t)4;

    res *= mult;
    return res;
}
