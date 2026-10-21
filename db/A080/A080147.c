/*
 * A080147  Positions of primes of the form 4*k+1 (A002144) among all primes
 *          (A000040).
 * https://oeis.org/A080147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the index k such that the k‑th prime ≡ 1 (mod 4)
 * Range:     u8 n <= 122, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;        /* offset is 1 – undefined for n = 0 */

    /* ----------------------------------------------------------
       On first call set up a dynamic array of primes that will
       be used to test further candidates via trial division.
       ---------------------------------------------------------- */
    static uint32_t *prime_list = NULL; /* list of primes found so far */
    static size_t    prime_cap  = 0;    /* allocated capacity */
    static size_t    prime_cnt  = 0;    /* number of primes stored   */

    /* ----------------------------------------------------------
       Generate primes incrementally until we’ve found the
       n‑th 1‑mod‑4 prime.  We keep two counters:
         pos   – index of the current prime in the entire prime list
         count – number of 1‑mod‑4 primes seen so far
       ---------------------------------------------------------- */
    uint64_t candidate = 2;            /* next integer to test */
    seq_t   pos        = 0;            /* position in full prime list   */
    seq_t   count      = 0;            /* count of 1‑mod‑4 primes        */

    for (;;)
    {
        /* ---- test candidate for primality ---- */
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i)
        {
            uint32_t p = prime_list[i];
            uint64_t pp = (uint64_t)p * (uint64_t)p;
            if (pp > candidate) break;
            if (candidate % p == 0) { is_prime = false; break; }
        }

        if (is_prime)
        {
            /* store the new prime (fits in uint32 for n up to large limits) */
            if (prime_cnt == prime_cap)
            {
                size_t new_cap = prime_cap == 0 ? 16 : prime_cap * 2;
                uint32_t *tmp = (uint32_t *)realloc(prime_list, new_cap * sizeof(uint32_t));
                if (!tmp) return 0;          /* allocation failed */
                prime_list = tmp;
                prime_cap  = new_cap;
            }
            prime_list[prime_cnt++] = (uint32_t)candidate;

            /* update position counter */
            ++pos;

            /* if this prime is 1 mod 4, increment count */
            if (candidate % 4 == 1)
            {
                ++count;
                if (count == (seq_t)n)
                    return pos;          /* desired term found */
            }
        }

        /* ---- advance to the next candidate ---- */
        if (candidate == 2)
            candidate = 3;
        else
            candidate += 2;              /* skip even numbers */
    }
}
