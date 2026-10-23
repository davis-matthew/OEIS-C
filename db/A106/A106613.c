/*
 * A106613  Nonprimes of the form r(r(r(n)+1)+1)+1, where A141468(n)=r(n)=n-th
 *          nonprime.
 * https://oeis.org/A106613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = r(r(r(n)+1)+1)+1, where r(k) is the k-th nonprime number
 *            (0,1,4,6,8,9,…). a(n) lists only those values that are
 *            themselves nonprime
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   isPrime64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/isPrime64.h"

/* Dynamic array of nonprime numbers r(k). */
static seq_t *r_seq = NULL;   /* r_seq[0] = r(1) = 0, r_seq[1] = r(2) = 1 */
static size_t r_len  = 0;
static size_t r_cap  = 0;
static uint64_t r_next = 0;   /* next integer to test for nonprime */

/* Allocate (or grow) the nonprime array.  */
static void ensure_r_cap(size_t min_cap)
{
    if (r_cap >= min_cap)
        return;
    size_t new_cap = r_cap ? r_cap * 2 : 64;
    while (new_cap < min_cap) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(r_seq, new_cap * sizeof(seq_t));
    r_seq = tmp;
    r_cap = new_cap;
}

/* Generate nonprimes until we have at least 'need' elements (1‑based). */
static void ensure_r(size_t need)
{
    if (r_len >= need)
        return;
    ensure_r_cap(need);
    /* Initialize the first two nonprimes if not done already. */
    if (r_len == 0)
    {
        r_seq[0] = 0;    /* r(1) */
        r_seq[1] = 1;    /* r(2) */
        r_len  = 2;
        r_next = 2;
    }
    while (r_len < need)
    {
        uint64_t val = r_next++;
        if (!isPrime64(val))
        {
            r_seq[r_len++] = (seq_t)val;
        }
    }
}

/* Return the k‑th nonprime r(k) (1‑based). */
static seq_t nth_nonprime(idx_t k)
{
    /* Needed indices are 1‑based, array is 0‑based. */
    if (k == 0) return 0;
    ensure_r((size_t)k);
    return r_seq[k - 1];
}

/* Main sequence generator.  */
seq_t a(idx_t n)
{
    /* Count how many nonprime results we have found. */
    size_t found = 0;
    idx_t pos   = 1;   /* current input index for r. */

    while (found < (size_t)n)
    {
        seq_t t1 = nth_nonprime(pos);                 /* r(n)   */
        seq_t t2 = nth_nonprime((idx_t)(t1 + 1));      /* r(r(n)+1) */
        seq_t t3 = nth_nonprime((idx_t)(t2 + 1));      /* r(r(r(n)+1)+1) */
        seq_t val = t3 + 1;                            /* a(n) candidate */

        /* val is nonprime if it is 0 or 1 or not prime. */
        if (val <= 1 || !isPrime64((uint64_t)val))
        {
            found++;
            if (found == (size_t)n)
                return val;
        }
        pos++;
    }
    /* Should never reach here. */
    return 0;
}
