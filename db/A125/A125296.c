/*
 * A125296  a(1)=1. a(n) is the number of earlier terms of the sequence which
 *          are noncomposites (1 or primes) and are coprime to n.
 * https://oeis.org/A125296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of earlier terms that are 1 or prime and coprime
 *            to n. a(1) = 1 For n > 1: a(n) = |{ m : 1 ≤ m < n, a(m) ∈ {1} ∪
 *            primes, gcd(a(m), n)=1 }|
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   gcd, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /*  static state to memoise computed terms  */
    static seq_t *cache = NULL;          /* holds a(i) for i = 1..size */
    static unsigned char *is_noncomp = NULL; /* 1 if a(i) is 1 or prime */
    static idx_t *noncomp_idx = NULL;   /* indices of non‑composite terms */
    static idx_t size = 0;              /* number of entries filled in cache  */
    static idx_t cap = 0;               /* allocated capacity of arrays       */
    static idx_t noncomp_cnt = 0;       /* number of non‑composite indices   */

    /* initialise once */
    if (cache == NULL) {
        cap = 256;                           /* small initial buffer */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        is_noncomp = (unsigned char *)malloc(cap * sizeof(unsigned char));
        noncomp_idx = (idx_t *)malloc(cap * sizeof(idx_t));
        /* a(1) = 1 */
        cache[0] = (seq_t)1;
        is_noncomp[0] = 1;                  /* 1 is considered non‑composite */
        noncomp_idx[0] = 1;
        noncomp_cnt = 1;
        size = 1;
    }

    /* if more space is needed, grow */
    if (n > cap) {
        idx_t newcap = cap;
        while (newcap < n) newcap *= 2;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        is_noncomp = (unsigned char *)realloc(is_noncomp, newcap * sizeof(unsigned char));
        noncomp_idx = (idx_t *)realloc(noncomp_idx, newcap * sizeof(idx_t));
        cap = newcap;
    }

    /* compute up to n if not already done */
    for (idx_t i = size + 1; i <= n; ++i) {
        /* count non‑composite earlier terms coprime to i */
        seq_t count = 0;
        seq_t i_seq = (seq_t)i;
        for (idx_t k = 0; k < noncomp_cnt; ++k) {
            seq_t a_val = cache[noncomp_idx[k] - 1];
            if (gcd(a_val, i_seq) == (seq_t)1)
                ++count;
        }
        cache[i - 1] = count;

        /* determine if a(i) is 1 or prime; store flag and list if so */
        if (count == (seq_t)1 || is_prime((idx_t)count)) {
            is_noncomp[i - 1] = 1;
            noncomp_idx[noncomp_cnt++] = i;
        } else {
            is_noncomp[i - 1] = 0;
        }
    }

    size = n > size ? n : size;
    return cache[n - 1];
}
