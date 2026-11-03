/*
 * A153779  Numbers of the form 49*k, where 49*k+2 and 49*k-6 are both prime.
 * https://oeis.org/A153779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is computed by searching for k such that 49*k-6 and 49*k+2
 *            are both prime
 * Range:     u8 none, u16 n <= 24, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 50 terms through n = 956 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External utilities used */
#include <stddef.h>                 /* size_t */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

/* Return the nth term of OEIS A153779 */
seq_t a(idx_t n)
{
    /* Static state for lazy computation */
    static seq_t  *seq    = NULL;   /* dynamic array of found terms */
    static size_t len    = 0;       /* number of terms stored */
    static size_t cap    = 0;       /* allocated capacity of seq */
    static idx_t  last_k = 0;       /* last k processed (starting from 0) */

    /* Ensure n >= 1 */
    if (n == 0) return (seq_t)0;    /* undefined, but avoid UB */

    /* Generate terms until we have at least n terms */
    while (len < (size_t)n) {
        /* Advance to the next candidate k */
        last_k++;

        /* Compute the candidate value 49*k */
        seq_t val = (seq_t)49 * (seq_t)last_k;

        /* Test the primality conditions */
        seq_t below = val - (seq_t)6;   /* 49*k-6 */
        seq_t above = val + (seq_t)2;   /* 49*k+2 */

        if (is_prime_big(below) && is_prime_big(above)) {
            /* Store the term */
            append_candidate(&seq, &len, &cap, val);
        }
        /* Continue until we reach the desired count */
    }

    /* Return the requested term (indexing from 1) */
    return seq[(size_t)(n - 1)];
}
