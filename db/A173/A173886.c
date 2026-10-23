/*
 * A173886  Fibonacci double product triangle:If[n == 1, 1, If[n == 0, 1,
 *          Product[Fibonacci[(i - 1)]*Fibonacci[i], {i, 2,
 *          n}]]];t(n,m)=c(n)/(c(m)*c(n-m)).
 * https://oeis.org/A173886
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, uned
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 28, u32 n <= 45, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A173886: Fibonacci double product triangle element
   a(n) = c(r) / (c(m) * c(r-m))
   where r = floor((sqrt(1+8n)-1)/2) and m = n - r(r+1)/2
   and c(k) = product_{i=2..k} Fib(i-1)*Fib(i) with c(0)=c(1)=1. */
#include <stdlib.h>
#include <math.h>

/* cached Fibonacci numbers and c values */
static seq_t *fib = NULL;
static size_t fib_cap = 0;
static idx_t fib_max = 0;

static seq_t *c = NULL;
static size_t c_cap = 0;
static idx_t c_max = 0;

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return 1;

    /* ------------------------------------------------------------------
     *  Mapping of linear index n to row r and column m
     * ------------------------------------------------------------------ */
    long double val = 1.0L + 8.0L * (long double)n;
    unsigned long long s   = (unsigned long long)floor(sqrtl(val));
    idx_t        r = (idx_t)((s - 1) >> 1);           /* floor((sqrt(1+8n)-1)/2)  */
    idx_t        prev_tri = r * (r + 1) / 2;          /* terms before row r    */
    idx_t        m = n - prev_tri;                   /* column in row r        */

    /* ------------------------------------------------------------------
     *  Initialize Fibonacci array if necessary
     * ------------------------------------------------------------------ */
    if (!fib) {
        fib = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!fib) return 0;          /* allocation failed – unlikely for the limits */
        fib[0] = 0;
        fib[1] = 1;
        fib_cap = 2;
        fib_max = 1;
    }

    /* Extend Fibonacci array to r */
    if (r > fib_max) {
        size_t new_cap = fib_cap;
        while (new_cap <= (size_t)r + 1)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(fib, new_cap * sizeof(seq_t));
        if (!tmp) return 0;
        fib = tmp;
        fib_cap = new_cap;
        for (idx_t i = fib_max + 1; i <= r; ++i)
            fib[i] = fib[i - 1] + fib[i - 2];
        fib_max = r;
    }

    /* ------------------------------------------------------------------
     *  Initialize c array if necessary
     * ------------------------------------------------------------------ */
    if (!c) {
        c = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!c) return 0;
        c[0] = 1;
        c[1] = 1;
        c_cap = 2;
        c_max = 1;
    }

    /* Extend c array to r */
    if (r > c_max) {
        size_t new_cap = c_cap;
        while (new_cap <= (size_t)r + 1)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(c, new_cap * sizeof(seq_t));
        if (!tmp) return 0;
        c = tmp;
        c_cap = new_cap;
        for (idx_t i = c_max + 1; i <= r; ++i) {
            /* c[i] = c[i-1] * fib[i-1] * fib[i]   for i>=2  */
            seq_t prod = fib[i - 1];
            prod *= fib[i];
            c[i] = c[i - 1] * prod;
        }
        c_max = r;
    }

    /* ------------------------------------------------------------------
     *  Compute the triangle entry: t(r,m) = c[r] / (c[m] * c[r-m])
     * ------------------------------------------------------------------ */
    seq_t term = c[r];
    term /= c[m];
    term /= c[r - m];
    return term;
}
