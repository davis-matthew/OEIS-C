/*
 * A109079  Numbers n such that the string n12341237 is prime.
 * https://oeis.org/A109079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer k such that 10^8*k + 12341237 is prime
 * Range:     u8 n <= 24, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   isPrime64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrime64.h"   /* bool isPrime64(uint64_t n); */

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;   /* stored terms */
    static idx_t size   = 0;      /* number stored */
    static idx_t cap    = 0;      /* current capacity */
    static idx_t next   = 0;      /* next candidate to test */

    /* 10^8 constant */
    const uint64_t base = 100000000ULL;
    const uint64_t offset = 12341237ULL;

    while (size < n) {
        /* build the candidate number */
        unsigned __int128 prod = (unsigned __int128)next * base + offset;
        uint64_t num = (uint64_t)prod;   /* fits in 64 bits for n up to ~1e10 */

        /* test primality */
        if (isPrime64(num)) {
            /* grow table if necessary */
            if (size == cap) {
                idx_t newcap = cap ? cap * 2 : 8;
#ifdef __cplusplus
                seq_t *newterms = new seq_t[newcap];
                for (idx_t i = 0; i < size; ++i)
                    newterms[i] = terms[i];
                delete[] terms;
#else
                seq_t *newterms = (seq_t *)malloc(newcap * sizeof(seq_t));
                for (idx_t i = 0; i < size; ++i)
                    newterms[i] = terms[i];
                free(terms);
#endif
                terms = newterms;
                cap = newcap;
            }
            terms[size++] = (seq_t)next;
        }
        ++next;
    }

    return terms[n - 1];
}
