/*
 * A143513  Numbers of the form 2^a * 3^b * 5^c * 17^d * 257^e * 65537^f;
 *          products of 2 and the Fermat primes.
 * https://oeis.org/A143513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 7731, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * The required OEIS formula: 
 *  a(n) = the nth smallest integer of the form 2^a * 3^b * 5^c * 17^d * 257^e * 65537^f
 *  where a,b,c,d,e,f ≥ 0 (offset 1 → a(1)=1).  
 * This uses the classic “super‑ugly‑number” algorithm with a static cache
 * so that any order of calls is supported.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Prime list whose powers generate the sequence. */
    static const seq_t PRIMES[6] = {
        (seq_t)2,
        (seq_t)3,
        (seq_t)5,
        (seq_t)17,
        (seq_t)257,
        (seq_t)65537
    };

    /* Static cache: ugly[i] holds a(i+1).  ugly[0] = 1. */
    static seq_t *ugly = NULL;
    static idx_t ugly_size = 0;   /* number of elements stored */
    static idx_t ugly_cap  = 0;   /* allocated capacity */

    /* Pointers for each prime – the classic super‑ugly algorithm. */
    static idx_t idx[6] = {0};

    if (n == 0) {            /* the sequence is defined for n ≥ 1 */
        return (seq_t)0;
    }

    /* First time: allocate and initialise the cache. */
    if (ugly_size == 0) {
        ugly_cap = 64;
        ugly = (seq_t *)malloc(ugly_cap * sizeof(seq_t));
        ugly[0] = (seq_t)1;       /* a(1) = 1 */
        ugly_size = 1;
    }

    /* Grow the cache until it covers the requested index. */
    while (ugly_size < n) {
        /* Increase capacity if needed. */
        if (ugly_size >= ugly_cap) {
            idx_t new_cap = ugly_cap * 2;
            if (new_cap == 0) new_cap = 64;
            ugly = (seq_t *)realloc(ugly, new_cap * sizeof(seq_t));
            ugly_cap = new_cap;
        }

        /* Find the minimal next candidate among the primes. */
        seq_t next = PRIMES[0] * ugly[idx[0]];
        for (size_t j = 1; j < 6; ++j) {
            seq_t cand = PRIMES[j] * ugly[idx[j]];
            if (cand < next) {
                next = cand;
            }
        }

        /* Store the next ugly number. */
        ugly[ugly_size++] = next;

        /* Advance all pointers that produced this value. */
        for (size_t j = 0; j < 6; ++j) {
            if (PRIMES[j] * ugly[idx[j]] == next) {
                ++idx[j];
            }
        }
    }

    return ugly[n - 1];
}
