/*
 * A128974  Numbers k such that 12k does not divide Fibonacci(12k).
 * https://oeis.org/A128974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 167, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A128974:
   Numbers k such that 12·k does NOT divide Fibonacci(12·k).
   The function computes the n‑th such k (n ≥ 1) by an explicit
   search.  For each candidate k we compute Fibonacci(12·k) modulo
   (12·k) using fast‑doubling; if the remainder is non‑zero, k is
   accepted.  Results are memoised so that each term is evaluated at
   most once. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Fast doubling:  computes Fn and Fn+1 modulo mod.  */
static void fib_mod(idx_t n, idx_t mod, idx_t *fn, idx_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    idx_t a, b;
    fib_mod(n >> 1, mod, &a, &b);            /* a = Fk , b = Fk+1 */

    idx_t two_b = (b + b) % mod;
    idx_t tmp;
    if (two_b >= a)
        tmp = two_b - a;
    else
        tmp = two_b + mod - a;               /* (2b - a) mod mod */

    idx_t c = (idx_t)(((unsigned __int128)a * tmp) % mod);
    idx_t d = (idx_t)((((unsigned __int128)a * a) + ((unsigned __int128)b * b)) % mod);

    if (n & 1) {
        *fn  = d;
        *fn1 = (c + d) % mod;
    } else {
        *fn  = c;
        *fn1 = d;
    }
}

static seq_t *queue = NULL;          /* memoised k values */
static size_t  qsize = 0;            /* number of stored terms   */
static size_t  qcap  = 0;            /* capacity of the array   */
static idx_t   last_tested = 0;      /* largest k that has been tested */

static void ensure_queue_capacity(size_t needed)
{
    if (needed <= qcap)
        return;
    size_t newcap = qcap == 0 ? 256 : qcap << 1;
    if (newcap < needed)
        newcap = needed;
    seq_t *newptr = (seq_t *)realloc(queue, newcap * sizeof(seq_t));
    if (!newptr)
        abort();                      /* out of memory */
    queue = newptr;
    qcap  = newcap;
}

seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for n = 0, but return 0 safely */
        return 0;

    /* generate terms until we have at least n of them */
    while (qsize < (size_t)n) {
        last_tested++;
        idx_t k   = last_tested;
        idx_t mod = 12 * k;

        idx_t f, dummy;
        fib_mod(12 * k, mod, &f, &dummy);   /* F(12k) mod (12k) */

        if (f != 0) {                       /* 12k does NOT divide F(12k) */
            ensure_queue_capacity(qsize + 1);
            queue[qsize++] = (seq_t)k;
        }
    }
    return queue[n - 1];
}
