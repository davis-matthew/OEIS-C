/*
 * A067528  Numbers n such that n - 4^k is a prime or 1 for all k > 0 and n >
 *          4^k.
 * https://oeis.org/A067528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 89, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = the nth integer n (n > 4^k for all k ≥ 1) such that
   n – 4^k is prime or 1 for every k ≥ 1 with 4^k < n. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

/* Helper: test whether n satisfies the defining property */
static bool good_candidate(uint64_t n)
{
    uint64_t pow4 = 4;                     /* 4^1 */
    while (pow4 < n)
    {
        uint64_t d = n - pow4;
        if (d != 1 && !is_prime_u64(d))
            return false;

        /* stop if next multiplication would overflow */
        if (pow4 > UINT64_MAX / 4)
            break;
        pow4 *= 4;                         /* next power of 4 */
    }
    return true;
}

/* seq_t a(idx_t n) - nth term of A067528 (1‑based) */
seq_t a(idx_t n)
{
    /* Undefined for n < 1 – return 0 for safety */
    if (n == 0)
        return (seq_t)0;

    /* Memoisation: dynamic array of computed terms */
    static seq_t *seq      = NULL;   /* array of terms */
    static size_t cap      = 0;      /* allocated capacity */
    static idx_t  len      = 0;      /* number of terms stored */
    static uint64_t next_cand = 5;   /* smallest candidate still to test */

    /* initial allocation */
    if (!seq)
    {
        cap = 32;
        seq = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* generate terms until we have at least n of them */
    while (len < n)
    {
        uint64_t cand = next_cand++;

        if (good_candidate(cand))
        {
            /* grow array if necessary */
            if ((size_t)len == cap)
            {
                cap <<= 1;
                seq = (seq_t *)realloc(seq, cap * sizeof(seq_t));
            }
            seq[len++] = (seq_t)cand;
        }
    }

    return seq[n - 1];   /* 1‑based indexing */
}
