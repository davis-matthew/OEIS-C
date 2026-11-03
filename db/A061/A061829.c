/*
 * A061829  Multiples of 5 having only odd digits.
 * https://oeis.org/A061829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10*A014261(n-1)+5 for n>1, where A014261(k) is the k‑th
 *            number composed solely of odd decimal digits. The special case
 *            a(1) = 5 uses the implicit value A014261(0)=0. This
 *            implementation builds terms sequentially and memoises results to
 *            allow arbitrary call order
 * Range:     u8 n <= 11, u16 n <= 531, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 65 terms through n = 9089 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Test whether all decimal digits of x are odd.  Assumes x>0. */
static bool all_odd_digits_seq_t(seq_t x)
{
    while (x > (seq_t)0) {
        seq_t d = x % (seq_t)10;
        if ((d & (seq_t)1) == (seq_t)0)      /* even digit found */
            return false;
        x /= (seq_t)10;
    }
    return true;
}

/* Return the next number after k that has all odd decimal digits.
   k is itself a number with all odd digits (except the special case k==0). */
static seq_t next_odd(seq_t k)
{
    if (k == (seq_t)0)          /* special starting point */
        return (seq_t)1;

    seq_t cand = k + (seq_t)2;   /* start with next odd number */
    while (!all_odd_digits_seq_t(cand))
        cand += (seq_t)2;
    return cand;
}

/* Public entry point defined by the judge. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;        /* undefined for index 0 */

    /* Static variables to memoise the sequence. */
    static seq_t *buf      = NULL;          /* array holding computed terms */
    static size_t buf_cap  = 0;              /* current capacity of buf     */
    static size_t buf_len  = 0;              /* number of terms stored      */
    static seq_t last_k    = (seq_t)0;       /* last odd-digit number used  */

    /* Ensure buffer has space for n terms. */
    if (buf_len < n) {
        size_t new_cap = buf_cap > 0 ? buf_cap : 16;
        while (new_cap < (size_t)n)
            new_cap <<= 1;
        void *p = mem_resize(seq_t, buf, new_cap);
        buf = (seq_t *)p;
        buf_cap = new_cap;
    }

    /* Generate terms up to n. */
    while (buf_len < n) {
        if (buf_len == 0) {                         /* a(1) */
            buf[0] = (seq_t)5;
            last_k  = (seq_t)0;                     /* implicit k for a(1) */
        } else {
            seq_t next_k = next_odd(last_k);
            buf[buf_len] = next_k * (seq_t)10 + (seq_t)5;
            last_k = next_k;
        }
        ++buf_len;
    }

    return buf[n - 1];
}
