/*
 * A095744  Primes p for which A037888(p) = 2, i.e., primes whose binary
 *          expansion needs flips of just two bits to become palindrome.
 * https://oeis.org/A095744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 1202, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_uint64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A095744: primes p such that the binary expansion
   requires exactly two bit flips to become a palindrome. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/bitlen_uint64.h"

seq_t a(idx_t n)
{
    /* static table of found primes, 1‑indexed from n = 1 */
    static seq_t *tbl = NULL;
    static size_t tbl_len = 0;   /* number of stored values */
    static size_t tbl_cap = 0;   /* allocated capacity */
    static idx_t cand = 2;       /* next integer to test as prime */

    /* generate values until we have at least n entries */
    while (tbl_len < n)
    {
        /* find next prime */
        while (!is_prime(cand))
            cand++;

        /* count mismatched symmetric bit pairs in the binary representation */
        uint64_t p = (uint64_t)cand;               /* primes fit in 64 bits */
        int     bits = bitlen_uint64(p);           /* number of bits */
        int     mism = 0;
        for (int i = 0; i < bits / 2 && mism <= 2; ++i)
        {
            uint64_t b1 = (p >> i) & 1ULL;
            uint64_t b2 = (p >> (bits - 1 - i)) & 1ULL;
            mism += b1 ^ b2;
        }

        /* store prime if exactly two mismatches are needed */
        if (mism == 2)
        {
            if (tbl_len >= tbl_cap)
            {
                size_t new_cap = tbl_cap ? tbl_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(tbl, new_cap * sizeof(seq_t));
                tbl = tmp;
                tbl_cap = new_cap;
            }
            tbl[tbl_len++] = (seq_t)cand;
        }

        cand++;   /* advance to next candidate */
    }

    return tbl[n - 1];   /* sequence is 1‑indexed */
}
