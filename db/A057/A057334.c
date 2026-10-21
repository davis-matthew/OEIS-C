/*
 * A057334  In A000120, replace each entry k with the k-th prime and replace 0
 *          with 1.
 * https://oeis.org/A057334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n==0)?1 : prime[popcount(n)-1]
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where prime[] is the list of primes (2,3,5,7,11,13,...) */

#include <stdint.h>

/* Prime table (first 64 primes, enough for any popcount of uint64_t) */
static const seq_t primes[] = {
    (seq_t)2,  (seq_t)3,  (seq_t)5,  (seq_t)7,  (seq_t)11, (seq_t)13,
    (seq_t)17, (seq_t)19, (seq_t)23, (seq_t)29, (seq_t)31, (seq_t)37,
    (seq_t)41, (seq_t)43, (seq_t)47, (seq_t)53, (seq_t)59, (seq_t)61,
    (seq_t)67, (seq_t)71, (seq_t)73, (seq_t)79, (seq_t)83, (seq_t)89,
    (seq_t)97, (seq_t)101,(seq_t)103,(seq_t)107,(seq_t)109,(seq_t)113,
    (seq_t)127,(seq_t)131,(seq_t)137,(seq_t)139,(seq_t)149,(seq_t)151,
    (seq_t)157,(seq_t)163,(seq_t)167,(seq_t)173,(seq_t)179,(seq_t)181,
    (seq_t)191,(seq_t)193,(seq_t)197,(seq_t)199,(seq_t)211,(seq_t)223,
    (seq_t)227,(seq_t)229,(seq_t)233,(seq_t)239,(seq_t)241,(seq_t)251,
    (seq_t)257,(seq_t)263,(seq_t)269,(seq_t)271,(seq_t)277,(seq_t)281,
    (seq_t)283,(seq_t)293,(seq_t)307,(seq_t)311
};

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)1;

    /* Count set bits in n */
    unsigned k = __builtin_popcountll((unsigned long long)n);

    /* k is at least 1 for n>0 */
    return primes[k - 1];
}
