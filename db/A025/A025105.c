/*
 * A025105  a(n) = s(1)s(n) + s(2)s(n-1) + ... + s(k)s(n-k+1), where k = [ n/2
 *          ], s = (F(2), F(3), F(4), ...).
 * https://oeis.org/A025105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1}^{⌊(n+1)/2⌋} F(i+1) * F((n+1)-i+2) where F(k) is
 *            the Fibonacci number (F(0)=0, F(1)=1). The offset on OEIS is 1,
 *            i.e. a(1) is the first listed term
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc, exit, EXIT_FAILURE */

static seq_t *fib_cache = NULL;   /* Fibonacci numbers, fib_cache[i] = F(i)   */
static idx_t  fib_len   = 0;      /* current allocated size of fib_cache   */

static seq_t *a_cache   = NULL;   /* computed a(n) values, a_cache[n] = a(n) */
static idx_t  a_len     = 0;      /* current allocated size of a_cache    */

/* Ensure fib_cache contains Fibonacci numbers up to index upto (inclusive). */
static void ensure_fibs(idx_t upto)
{
    if (fib_len > upto) return;

    idx_t new_len = upto + 1;   /* indices 0 .. upto inclusive */
    seq_t *tmp = (seq_t *)realloc(fib_cache, new_len * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);

    for (idx_t i = fib_len; i <= upto; ++i) {
        if (i == 0)          tmp[i] = (seq_t)0;
        else if (i == 1)     tmp[i] = (seq_t)1;
        else                 tmp[i] = tmp[i-1] + tmp[i-2];
    }
    fib_cache = tmp;
    fib_len   = new_len;
}

/* Compute and store a(n) for all n up to `upto`. */
static void ensure_a(idx_t upto)
{
    if (a_len > upto) return;                /* nothing to do */

    idx_t new_len = upto + 1;                /* indices 0 .. upto inclusive */
    seq_t *tmp = (seq_t *)realloc(a_cache, new_len * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);

    /* First element is unused; set to 0 for completeness. */
    if (a_len == 0) tmp[0] = (seq_t)0;

    for (idx_t cur = a_len; cur <= upto; ++cur) {
        /* cur >= 1 for real sequence values. */
        idx_t n1 = cur + 1;                      /* use shifted index */
        idx_t k  = n1 / 2;                       /* floor */
        ensure_fibs(n1 + 1);                     /* need F(n1+1) */

        seq_t sum = 0;
        for (idx_t i = 1; i <= k; ++i) {
            seq_t term = fib_cache[i + 1] * fib_cache[n1 - i + 2];
            sum += term;
        }
        tmp[cur] = sum;
    }
    a_cache = tmp;
    a_len   = new_len;
}

/* Public entry point. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1, but safe */
    ensure_a(n);
    return a_cache[n];
}
