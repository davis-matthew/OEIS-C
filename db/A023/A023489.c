/*
 * A023489  Sum of n-th Lucas number greater than 3 and n-th number that is 1
 *          or is not a Fibonacci number.
 * https://oeis.org/A023489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = L(n+2) + A001690(n) where L is the Lucas sequence
 *            (L(0)=2, L(1)=1) and A001690(n) is the n-th number that is 1 or
 *            not a Fibonacci number
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required headers */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------- Fibonacci pair (F(n),F(n+1)) by fast doubling */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);          /* a=F(k), b=F(k+1) , k=floor(n/2) */

    seq_t two_b      = b + b;           /* 2*F(k+1)                     */
    seq_t temp       = two_b - a;       /* 2*F(k+1) - F(k)              */
    seq_t c          = a * temp;        /* F(2k) = F(k)*(2*F(k+1)-F(k))*/
    seq_t aa         = a * a;           /* F(k)^2                       */
    seq_t bb         = b * b;           /* F(k+1)^2                     */
    seq_t d          = aa + bb;         /* F(2k+1) = F(k)^2+F(k+1)^2    */

    if ((n & 1) == 0) {                 /* n = 2k */
        *fn  = c;
        *fn1 = d;
    } else {                            /* n = 2k+1 */
        *fn  = d;
        *fn1 = c + d;
    }
}

/* --------------------------------- Lucas number L(n)  (n >= 0) */
static seq_t lucas(idx_t n)
{
    if (n == 0) return 2;
    if (n == 1) return 1;
    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);            /* fn  = F(n)   */
                                       /* fn1 = F(n+1) */
    seq_t two_fn1 = fn1 + fn1;         /* 2*F(n+1)    */
    return two_fn1 - fn;               /* L(n) = 2*F(n+1) - F(n) */
}

/* --------------------------------- Non‑Fibonacci cache  (A001690) */
static seq_t *nonfib_cache = NULL;
static size_t nonfib_size = 0;
static size_t nonfib_cap  = 0;
static bool    nonfib_initialized = false;
static idx_t  cur_test_int = 1;       /* next integer to test for non‑Fibonacci */
static idx_t  fib_prev = 1, fib_curr = 1;  /* for generating Fibonacci > 1 */
static idx_t  next_fib = 2;          /* next Fibonacci number to skip     */

static void ensure_nonfib(size_t needed)
{
    if (nonfib_size >= needed)
        return;

    if (!nonfib_initialized) {
        nonfib_initialized = true;
        fib_prev  = 1;
        fib_curr  = 1;
        next_fib  = 2;     /* first Fibonacci > 1 */
        cur_test_int = 1;  /* start testing from 1 */
    }

    if (nonfib_cap == 0) {
        nonfib_cap = 8;
        nonfib_cache = (seq_t *)malloc(nonfib_cap * sizeof(seq_t));
        if (!nonfib_cache) exit(1);
    }

    while (nonfib_size < needed) {
        /* store 1 explicitly */
        if (cur_test_int == 1) {
            if (nonfib_size >= nonfib_cap) {
                size_t new_cap = nonfib_cap * 2;
                seq_t *tmp = (seq_t *)realloc(nonfib_cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);
                nonfib_cache = tmp;
                nonfib_cap  = new_cap;
            }
            nonfib_cache[nonfib_size++] = 1;
            cur_test_int++;
            continue;
        }

        /* skip Fibonacci numbers (>1) */
        if (cur_test_int == next_fib) {
            idx_t tmp = fib_prev + fib_curr; /* new Fibonacci number */
            fib_prev = fib_curr;
            fib_curr = tmp;
            next_fib = fib_prev + fib_curr;
            cur_test_int++;
            continue;
        }

        /* non‑Fibonacci number */
        if (nonfib_size >= nonfib_cap) {
            size_t new_cap = nonfib_cap * 2;
            seq_t *tmp = (seq_t *)realloc(nonfib_cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            nonfib_cache = tmp;
            nonfib_cap  = new_cap;
        }
        nonfib_cache[nonfib_size++] = (seq_t)cur_test_int;
        cur_test_int++;
    }
}

/* --------------------------------- Public sequence function */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)                 /* below the offset – arbitrary */
        return 0;

    seq_t luc = lucas(n + 2);          /* L(n+2)  */
    ensure_nonfib((size_t)n);
    seq_t nf  = nonfib_cache[(size_t)n - 1];
    return luc + nf;                   /* a(n) = L(n+2) + A001690(n) */
}
