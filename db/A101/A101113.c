/*
 * A101113  Let t(G) = number of unitary factors of the Abelian group G. Then
 *          a(n) = Sum t(G) over all Abelian groups G of order exactly n.
 * https://oeis.org/A101113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, easy, nonn
 * Formula:   a(n) = 2^ω(n) * ∏p partition(exp_p(n))
 * Range:     u8 n <= 6911, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Where ω(n) is number of distinct prime factors of n, 
   exp_p(n) is exponent of prime p in factorization of n,
   and partition(k) is the number of integer partitions of k. */

#include <stdint.h>
#include "../utils/int_sqrt.h"

/* Precomputed partition numbers p(k) for k=0..64 */
static const seq_t PARTITION[65] = {
    1ULL,   /* p(0) = 1 (by convention) */
    1ULL,   /* p(1) = 1 */
    2ULL,   /* p(2) = 2 */
    3ULL,   /* p(3) = 3 */
    5ULL,   /* p(4) = 5 */
    7ULL,   /* p(5) = 7 */
    11ULL,  /* p(6) = 11 */
    15ULL,  /* p(7) = 15 */
    22ULL,  /* p(8) = 22 */
    30ULL,  /* p(9) = 30 */
    42ULL,  /* p(10) = 42 */
    56ULL,  /* p(11) = 56 */
    77ULL,  /* p(12) = 77 */
    101ULL, /* p(13) = 101 */
    135ULL, /* p(14) = 135 */
    176ULL, /* p(15) = 176 */
    231ULL, /* p(16) = 231 */
    297ULL, /* p(17) = 297 */
    385ULL, /* p(18) = 385 */
    490ULL, /* p(19) = 490 */
    627ULL, /* p(20) = 627 */
    792ULL, /* p(21) = 792 */
    1002ULL,/* p(22) = 1002 */
    1255ULL,/* p(23) = 1255 */
    1575ULL,/* p(24) = 1575 */
    1958ULL,/* p(25) = 1958 */
    2436ULL,/* p(26) = 2436 */
    3010ULL,/* p(27) = 3010 */
    3718ULL,/* p(28) = 3718 */
    4565ULL,/* p(29) = 4565 */
    5604ULL,/* p(30) = 5604 */
    6842ULL,/* p(31) = 6842 */
    8316ULL,/* p(32) = 8316 */
    10022ULL,/* p(33) = 10022 */
    12144ULL,/* p(34) = 12144 */
    14591ULL,/* p(35) = 14591 */
    17496ULL,/* p(36) = 17496 */
    20874ULL,/* p(37) = 20874 */
    24741ULL,/* p(38) = 24741 */
    29172ULL,/* p(39) = 29172 */
    34278ULL,/* p(40) = 34278 */
    40380ULL,/* p(41) = 40380 */
    47595ULL,/* p(42) = 47595 */
    56092ULL,/* p(43) = 56092 */
    66012ULL,/* p(44) = 66012 */
    77518ULL,/* p(45) = 77518 */
    90846ULL,/* p(46) = 90846 */
    106385ULL,/* p(47) = 106385 */
    124035ULL,/* p(48) = 124035 */
    144540ULL,/* p(49) = 144540 */
    167763ULL,/* p(50) = 167763 */
    194004ULL,/* p(51) = 194004 */
    223577ULL,/* p(52) = 223577 */
    256731ULL,/* p(53) = 256731 */
    293748ULL,/* p(54) = 293748 */
    334931ULL,/* p(55) = 334931 */
    380508ULL,/* p(56) = 380508 */
    430836ULL,/* p(57) = 430836 */
    486226ULL,/* p(58) = 486226 */
    547017ULL,/* p(59) = 547017 */
    613581ULL,/* p(60) = 613581 */
    686328ULL,/* p(61) = 686328 */
    765700ULL,/* p(62) = 765700 */
    852172ULL,/* p(63) = 852172 */
    946259ULL  /* p(64) = 946259 */
};

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    seq_t result = 1ULL;
    idx_t temp = n;

    /* Handle factor 2 separately to avoid even loop costs */
    if ((temp & 1ULL) == 0) {
        idx_t exp = 0;
        while ((temp & 1ULL) == 0) {
            temp >>= 1;
            exp++;
        }
        result <<= 1;            /* multiply by 2^1 for this prime */
        if (exp <= 64) result *= PARTITION[exp];
    }

    /* Remaining odd factors */
    idx_t limit = int_sqrt(temp);
    for (idx_t p = 3; p <= limit; p += 2) {
        if (temp % p == 0) {
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            result <<= 1;            /* unitary factor contribution */
            if (exp <= 64) result *= PARTITION[exp];
            limit = int_sqrt(temp);
        }
    }

    /* If a prime factor > sqrt remains */
    if (temp > 1) {
        /* Remaining exponent is 1 */
        result <<= 1;            /* multiply by 2 for this prime */
        result *= PARTITION[1];   /* partition(1) = 1, but keep for consistency */
    }

    return result;
}
