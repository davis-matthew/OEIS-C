/*
 * A075731  Fibonacci numbers F(k) for k squarefree (A005117).
 * https://oeis.org/A075731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Fibonacci_k where k is the nth squarefree integer
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the sequence A075731:
   a(n) = Fib(S(n))   with S(1)=1, S(2)=2, S(3)=3, S(4)=5, ... the list of
   squarefree positive integers.  Fib(1)=1, Fib(2)=1, Fib(3)=2, ... */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Helper: fast‑doubling Fibonacci.  On return fn=F(n), fn1=F(n+1).  */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn = (seq_t)0;
        *fn1 = (seq_t)1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);
    seq_t two_b = b + b;
    seq_t two_b_minus_a = two_b - a;
    seq_t c = a * two_b_minus_a;
    seq_t d = a * a + b * b;
    if (n & 1) {
        *fn = d;
        *fn1 = c + d;
    } else {
        *fn = c;
        *fn1 = d;
    }
}

/* Storage for squarefree numbers (built lazily). */
static bool *sqfree_flag = NULL;          /* true if i is squarefree */
static idx_t sqfree_limit = 0;           /* maximum i processed */
static idx_t *sqfree_list = NULL;        /* array of squarefree numbers */
static size_t sqfree_count = 0;          /* number of squarefree numbers stored */
static size_t sqfree_capacity = 0;       /* allocated capacity of sqfree_list */

static void ensure_squarefree(idx_t need)
{
    if (sqfree_count >= need) return;

    /* Choose new limit large enough for 'need' squarefree numbers.
       Rough estimate: density ~ 6/π² ≈ 0.6079. */
    idx_t new_limit = (idx_t)(need * 2 + 100);
    while ((idx_t)(new_limit * 0.6079 + 10) < need) new_limit *= 2;

    /* Recompute flag array from scratch up to new_limit. */
    bool *flag = (bool *)malloc((new_limit + 1) * sizeof(bool));
    if (!flag) abort();                 /* allocation failure */
    for (idx_t i = 0; i <= new_limit; ++i) flag[i] = true;
    for (idx_t p = 2; p * p <= new_limit; ++p) {
        idx_t sq = p * p;
        for (idx_t j = sq; j <= new_limit; j += sq) flag[j] = false;
    }

    /* Build list of squarefree numbers. */
    size_t new_cap = (size_t)(new_limit);
    idx_t *list = (idx_t *)realloc(sqfree_list, new_cap * sizeof(idx_t));
    if (!list) abort();
    size_t cnt = 0;
    for (idx_t i = 1; i <= new_limit; ++i) {
        if (flag[i]) {
            if (cnt == new_cap) {
                /* double capacity if needed */
                new_cap *= 2;
                list = (idx_t *)realloc(list, new_cap * sizeof(idx_t));
                if (!list) abort();
            }
            list[cnt++] = i;
        }
    }

    /* Clean up old data and install new data. */
    free(sqfree_flag);
    free(sqfree_list);
    sqfree_flag = flag;
    sqfree_list = list;
    sqfree_limit = new_limit;
    sqfree_count = cnt;
    sqfree_capacity = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* out of range; not used */
    ensure_squarefree(n);
    idx_t k = sqfree_list[n - 1];        /* nth squarefree integer */
    seq_t fn, fnp1;
    fib_pair(k, &fn, &fnp1);             /* fn = F(k) */
    return fn;
}
