/*
 * A156260  Row sums of A156254.
 * https://oeis.org/A156260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^ω(n) if n is squarefree, otherwise 1 where ω(n) is the
 *            number of distinct prime factors of n
 * Range:     u8 n <= 1500, u16 n <= 1500, u32 n <= 1500, u64 n <= 1500,
 *            u128 n <= 1500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h> /* for bool */

seq_t a(idx_t n)
{
    if (n < 1)                /* safety; not used for the official offset */
        return (seq_t)0;

    idx_t tmp = n;
    unsigned int distinct = 0;
    bool squarefree = true;

    /* factor 2 explicitly */
    if (tmp % 2 == 0) {
        int exp = 0;
        do {
            tmp /= 2;
            exp++;
            if (exp > 1)
                squarefree = false;
        } while (tmp % 2 == 0);
        if (exp == 1)
            distinct++;
    }

    /* factor odd primes */
    for (idx_t i = 3; i <= tmp / i; i += 2) {
        if (tmp % i == 0) {
            int exp = 0;
            do {
                tmp /= i;
                exp++;
                if (exp > 1)
                    squarefree = false;
            } while (tmp % i == 0);
            if (exp == 1)
                distinct++;
        }
    }

    /* remaining factor > sqrt(n) */
    if (tmp > 1)
        distinct++;

    if (!squarefree)
        return (seq_t)1;
    else
        return ((seq_t)1 << distinct);
}
