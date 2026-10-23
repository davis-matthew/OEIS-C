/*
 * A104488  Number of Hamiltonian groups of order n.
 * https://oeis.org/A104488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 0 if e(n)<3 (where n=2^e⋅o) else a(o) = ∏_{p^e||o}
 *            partition(e), where partition(e) is the number of integer
 *            partitions of e
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Auxiliary array of partition numbers up to 64:
   p(0)=1, p(1)=1, p(2)=2, …, p(64)=1657925. */
static const seq_t partition_table[65] = {
    1, 1, 2, 3,   5,   7,   11,  15,  22, 30, 42, 56, 77, 101, 135, 176,
   231, 297, 385, 490, 627, 792, 1002,1255,1575,1958,2436,3010,3718,
   4565,5604,6842,8349,10143,12310,14883,17977,21637,26015,31160,
   37347,44622,53229,63261,75030,88745,104752,123500,145420,170885,
   200382,234426,274603,321549,376017,438838,511991,596589,694033,
   806010,934352,1080999,1246304,1437532,1657925
};

seq_t a(idx_t n)
{
    /* Count the exponent of 2 in n.  n ≥ 1. */
    unsigned e = __builtin_ctzll(n);

    /* If the power of 2 is less than 8, there are no Hamiltonian groups. */
    if (e < 3)
        return 0;

    /* Extract the odd part. */
    idx_t odd = n >> e;

    /* If odd part is 1, its factorisation contributes a product of 1. */
    if (odd == 1)
        return 1;

    seq_t result = 1;
    idx_t t = odd;

    /* Trial division by odd primes. */
    for (idx_t p = 3; p * p <= t; p += 2) {
        if (t % p == 0) {
            unsigned exp = 0;
            while (t % p == 0) {
                t /= p;
                exp++;
            }
            /* Multiply by the partition number for this exponent. */
            result *= (seq_t)partition_table[exp];
        }
    }

    /* Remaining prime factor (if any) has exponent 1, partition(1)=1. */
    return result;
}
