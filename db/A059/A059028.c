/*
 * A059028  Row sums of A059026: a(n) = Sum_{m=1..n} (lcm(n,m)/n + lcm(n,m)/m
 *          - 1).
 * https://oeis.org/A059028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{m=1..n} (lcm(n,m)/n + lcm(n,m)/m - 1)
 * Range:     u8 n <= 16, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   gen_divs_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gen_divs_phi.h"

/* Small prime table sufficient for n <= 10 000 */
static const unsigned prime_list[] =
{2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,
 101,103,107,109,113,127,131,137,139,149,151,157,163,167,173,179,181,
 191,193,197,199,211,223,227,229,233,239,241,251,257,263,269,271,277,
 281,283,293,307,311,313,317,331,337,347,349,353,359,367,373,379,383,
 389,397,401,409,419,421,431,433,439,443,449,457,461,463,467,479,487,
 491,499,503,509,521,523,541,547,557,563,569,571,577,587,593,599,601,
 607,613,617,619,631,641,643,647,653,659,661,673,677,683,691,701,709,
 719,727,733,739,743,751,757,761,769,773,787,797,809,811,821,823,827,
 829,839,853,857,859,863,877,881,883,887,907,911,919,929,937,941,947,
 953,967,971,977,983,991,997};
static const size_t PRIME_COUNT = sizeof(prime_list)/sizeof(prime_list[0]);

/* Factor an integer into prime powers using trial division. */
static void
factor_uint64(uint64_t n, unsigned *primes, unsigned *exps, size_t *cnt)
{
    *cnt = 0;
    if (n <= 1) return;
    for (size_t i = 0; i < PRIME_COUNT && (uint64_t)prime_list[i] * prime_list[i] <= n; ++i) {
        if (n % prime_list[i] == 0) {
            unsigned p = prime_list[i];
            unsigned e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            primes[(*cnt)] = p;
            exps[(*cnt)] = e;
            ++(*cnt);
        }
    }
    if (n > 1) {
        primes[(*cnt)] = (unsigned)n;
        exps[(*cnt)]   = 1u;
        ++(*cnt);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* offset starts at 1 */

    /* factor n */
    unsigned primes[16];
    unsigned exps[16];
    size_t pcnt;
    factor_uint64((uint64_t)n, primes, exps, &pcnt);

    /* maximum divisors for n <= 10 000 is < 128 */
    uint64_t divs[128];
    uint64_t phis[128];
    size_t dcnt = 0;
    gen_divs_phi(primes, exps, pcnt, 0, (uint64_t)1, (uint64_t)1,
                 divs, phis, &dcnt);

    seq_t S1 = 0;
    seq_t S2 = 0;
    for (size_t i = 0; i < dcnt; ++i) {
        uint64_t t      = divs[i];
        uint64_t phi_t  = phis[i];
        if (t == 1) {
            S1 += (seq_t)1;                     /* sum_{k} k = 1 */
        } else {
            S1 += ((seq_t)t * (seq_t)phi_t) / (seq_t)2;
        }
        S2 += (seq_t)t * (seq_t)phi_t;
    }

    seq_t result = S1 + S2 - (seq_t)n;
    return result;
}
