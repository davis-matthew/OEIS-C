/*
 * A219196  A subsequence of the denominators of the Bernoulli numbers: a(n) =
 *          A027642(A131577(n)).
 * https://oeis.org/A219196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of the first m Fermat primes, where m = min(
 *            floor(log2(n-1)) + 2, 6 ) for n ≥ 2, and a(0) = 1, a(1) = 2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The Fermat primes are 2, 3, 5, 17, 257, 65537.
   Their cumulative products form the known terms of the sequence. */

#include <stdbool.h>
#include <stdint.h>

static const seq_t partial_product[7] = {
    (seq_t)1ULL,            /* 0 primes  : 1          */
    (seq_t)2ULL,            /* 1 prime   : 2          */
    (seq_t)6ULL,            /* 2 primes  : 2*3        */
    (seq_t)30ULL,           /* 3 primes  : 2*3*5      */
    (seq_t)510ULL,          /* 4 primes  : 2*3*5*17   */
    (seq_t)131070ULL,       /* 5 primes  : 2*3*5*17*257 */
    (seq_t)8589934590ULL    /* 6 primes  : 2*3*5*17*257*65537 */
};

seq_t a(idx_t n)
{
    /* Handle the initial offsets explicitly. */
    if (n == 0) return partial_product[0];
    if (n == 1) return partial_product[1];

    /* For n ≥ 2: determine how many Fermat primes to multiply. */
    idx_t t = n - 1;        /* t ≥ 1 */
    int exp = 0;
    while (t > 1) {         /* compute floor(log2(t)) */
        t >>= 1;
        exp++;
    }

    /* Number of Fermat primes required. */
    idx_t m = (idx_t)(exp + 2);  /* floor(log2(n-1)) + 2 */

    /* No further Fermat primes are known, so cap at 6. */
    if (m > 6) m = 6;

    return partial_product[m];
}
