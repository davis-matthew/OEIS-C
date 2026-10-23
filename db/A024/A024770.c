/*
 * A024770  Right-truncatable primes: every prefix is prime.
 * https://oeis.org/A024770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, fini, full, nice, tabf
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 34, u32 n <= 55, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate all right‑truncatable primes.
   A right‑truncatable prime is a prime such that repeatedly deleting
   the least significant digit leaves a prime at every step until a
   single‑digit prime remains.  The sequence has exactly 83 terms.  The
   algorithm starts with the single‑digit primes and iteratively
   appends digits 0‑9, keeping only the results that are prime, then
   sorts the list.  The result is cached so that arbitrary‑ordered
   calls to a(idx_t) are O(1).  The implementation is portable to C11
   and C++17.  For C++ the array is allocated with new[] (to call
   constructors of an arbitrary‑precision type); for C it uses
   malloc(). */

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

/* Utility headers provided by the build system. */
#include "../utils/is_prime_big.h"

/* Comparator for qsort: works for integral and arbitrary‑precision types. */
static int cmp_seq(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* Static cache of right‑truncatable primes. */
static seq_t *rt_primes = NULL;
static size_t rt_len   = 0;
static const size_t RT_CAP = 128;          /* sufficient for all 83 primes */

/* Build the cache once.  Subsequent calls are no‑ops. */
static void build_cache(void)
{
    if (rt_primes != NULL) return;         /* already built */

    /* Allocate storage.  In C++ we use new[] to construct the objects,
       in C we use malloc. */
#ifdef __cplusplus
    rt_primes = new seq_t[RT_CAP];
#else
    rt_primes = (seq_t *)malloc(RT_CAP * sizeof(seq_t));
    if (!rt_primes) { rt_len = 0; return; }   /* allocation failed */
#endif

    rt_len = 0;

    /* Start with the single‑digit primes. */
    const seq_t start_primes[4] = {2, 3, 5, 7};
    for (size_t i = 0; i < 4; ++i)
        rt_primes[rt_len++] = start_primes[i];

    /* Expand by appending each digit 0–9 to existing primes. */
    for (size_t idx = 0; idx < rt_len; ++idx)
    {
        seq_t p = rt_primes[idx];
        for (int d = 0; d <= 9; ++d)
        {
            seq_t q = p * (seq_t)10 + (seq_t)d;
            if (is_prime_big(q))
            {
                rt_primes[rt_len++] = q;
                /* rt_len will never exceed RT_CAP (there are only 83
                   primes). */
            }
        }
    }

    /* Sort so that a(1)=2, a(2)=3, … */
    qsort(rt_primes, rt_len, sizeof(seq_t), cmp_seq);
}

/* Public sequence function.  Returns 0 for indices > 83. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* index before offset */

    build_cache();                 /* ensure cache is ready */

    if ((idx_t)n > rt_len) return (seq_t)0;  /* sequence ends */

    return rt_primes[n - 1];
}
