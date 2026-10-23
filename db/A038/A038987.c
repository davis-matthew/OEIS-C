/*
 * A038987  Primes p such that 97 is a square mod p.
 * https://oeis.org/A038987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A038987: Primes p such that 97 is a quadratic residue modulo p
   a(1) = 2, a(2) = 3, a(3) = 11, a(4) = 31, …                                     */
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* Pre‑compute quadratic residues modulo 97 */
    static bool residues[97];
    static bool init = false;

    /* Cache of primes up to a generous upper bound */
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;
    static uint64_t last_known = 2;   /* last prime we have processed */

    if (!init)
    {
        /* Marks all squares mod 97 */
        for (int i = 0; i < 97; ++i)
            residues[(i * i) % 97] = true;

        /* Enough to cover the first ~10k terms (≈200k entries) */
        uint64_t limit = 2000000;
        build_primes_upto(limit, &primes, &primes_cnt);
        last_known = primes[primes_cnt - 1];
        init = true;
    }

    /* Count satisfied primes while iterating over the pre‑computed list */
    size_t count = 0;
    for (size_t i = 0; i < primes_cnt; ++i)
    {
        uint64_t p = primes[i];
        bool good = false;
        if (p == 2 || p == 97)
            good = true;
        else if (residues[p % 97])
            good = true;

        if (good)
        {
            ++count;
            if (count == n)
                return (seq_t)p;
        }
    }

    /* If we run out of pre‑computed primes, keep searching further */
    uint64_t p = last_known + 1;
    while (true)
    {
        /* Find the next prime greater than p */
        while (!is_prime_big((seq_t)p))
            ++p;
        last_known = p;

        bool good = false;
        if (p == 97)
            good = true;
        else if (residues[p % 97])
            good = true;

        if (good)
        {
            ++count;
            if (count == n)
                return (seq_t)p;
        }
        ++p;
    }
}
