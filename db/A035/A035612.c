/*
 * A035612  Horizontal para-Fibonacci sequence: says which column of Wythoff
 *          array (starting column count at 1) contains n.
 * https://oeis.org/A035612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = index of the smallest Fibonacci number in the Zeckendorf
 *            representation of n, where the Fibonacci sequence is defined by
 *            f(1)=1, f(2)=2, f(i)=f(i-1)+f(i-2). This is equivalent to the
 *            OEIS A035612
 * Range:     u8 n <= 357, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

static seq_t * _fib_cache = NULL;   /* 1‑based array of Fibonacci numbers */
static idx_t  _fib_cap   = 0;        /* capacity of _fib_cache */
static idx_t  _fib_n     = 0;        /* largest index currently stored */

/* Ensure that _fib_cache contains Fibonacci numbers f(1)=1, f(2)=2,
   ... up to an index such that f(_fib_n) > n.  */
static void _ensure_fibs_upto(idx_t n)
{
    seq_t target = (seq_t)n;

    /* Initialise cache if necessary */
    if (_fib_cache == NULL) {
        _fib_cap   = 16;
        _fib_cache = (seq_t *)malloc(_fib_cap * sizeof(seq_t));
        if (_fib_cache == NULL) return;               /* allocation failed, caller will handle */
        _fib_cache[1] = (seq_t)1;  /* f(1) = 1 */
        _fib_cache[2] = (seq_t)2;  /* f(2) = 2 */
        _fib_n       = 2;
    }

    /* Grow capacity if we need to store more Fibonacci numbers */
    if (_fib_n + 1 >= _fib_cap) {
        size_t newcap = _fib_cap * 2;
        seq_t *tmp = (seq_t *)realloc(_fib_cache, newcap * sizeof(seq_t));
        if (tmp == NULL) return;                       /* realloc failed */
        _fib_cache = tmp;
        _fib_cap   = newcap;
    }

    /* Generate Fibonacci numbers until we reach or exceed target */
    while (_fib_cache[_fib_n] <= target) {
        /* Compute next Fibonacci number */
        _fib_cache[_fib_n + 1] = _fib_cache[_fib_n] + _fib_cache[_fib_n - 1];
        _fib_n++;

        /* Ensure we still have enough capacity */
        if (_fib_n + 1 >= _fib_cap) {
            size_t newcap = _fib_cap * 2;
            seq_t *tmp = (seq_t *)realloc(_fib_cache, newcap * sizeof(seq_t));
            if (tmp == NULL) return;                     /* realloc failed */
            _fib_cache = tmp;
            _fib_cap   = newcap;
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* safeguard for n=0 (not used in tests) */

    _ensure_fibs_upto(n);

    seq_t remaining = (seq_t)n;
    seq_t min_index = (seq_t)0;          /* will be set when we first pick a fib */

    /* Greedy Zeckendorf subtraction from the largest Fibonacci downwards. */
    for (idx_t i = _fib_n; i >= 1; --i) {
        if (_fib_cache[i] <= remaining) {
            remaining -= _fib_cache[i];
            min_index = (seq_t)i;         /* record the smallest index so far */
            if (remaining == 0) break;    /* finished early */
        }
    }
    return min_index;
}
