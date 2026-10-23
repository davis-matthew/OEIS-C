/*
 * A034747  Dirichlet convolution of Fibonacci numbers with sigma(n).
 * https://oeis.org/A034747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} σ(d) · F(n/d), n ≥ 1
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  where σ is the divisor–sum function and F is the Fibonacci sequence
    with F(1)=1, F(2)=1.  This implementation memoises σ and F values
    up to the largest n requested so far.  An O(√n) divisor walk then
    computes the convolution value. */

#include <stdlib.h>

/* Forward declarations from the build system –  do NOT replace these. */
extern seq_t a(idx_t n);          /* the function to be defined below */

/* Static cache for σ(k) and F(k).  The arrays are valid for indices 0..max_prepared. */
static seq_t *sigma_arr = NULL;   /* σ[0] unused, σ[1]..σ[max_prepared] present */
static seq_t *fib_arr   = NULL;   /* fib[0] unused, fib[1]..fib[max_prepared] present */
static idx_t max_prepared = 0;    /* largest index currently stored */

/* Integer square root of an idx_t value (returns floor(sqrt(n))). */
static idx_t isqrt_idx(idx_t n)
{
    idx_t r = 0;
    /* classic method: increment r while (r+1)^2 <= n */
    while (r + 1 <= n / (r + 1))
        ++r;
    return r;
}

/* Ensure σ and Fibonacci arrays are filled for all indices up to n (inclusive).
   If n is larger than the current capacity, free the old arrays and allocate
   new ones of size n + 1.  Complexity O(n log n) for the σ sieve and O(n)
   for Fibonacci generation. */
static void prepare_upto(idx_t n)
{
    if (n <= max_prepared)
        return;                                 /* already prepared */

    /* Free any previous allocations */
    free(sigma_arr);
    free(fib_arr);

    /* Allocate new arrays.  Guard against overflow of the size_t argument. */
    if (n > SIZE_MAX - 1)
        return;                                 /* out of memory: abort */

    sigma_arr = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    fib_arr   = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!sigma_arr || !fib_arr)
        return;                                 /* allocation failed */

    /* Initialise all entries to zero. */
    for (idx_t i = 0; i <= n; ++i) {
        sigma_arr[i] = (seq_t)0;
        fib_arr[i]   = (seq_t)0;
    }

    /* Compute σ(k) via a simple sieve: for each divisor d add d to all multiples. */
    for (idx_t d = 1; d <= n; ++d) {
        seq_t val = (seq_t)d;                   /* σ contribution of d */
        for (idx_t m = d; m <= n; m += d)
            sigma_arr[m] += val;
    }

    /* Compute Fibonacci numbers up to n (F(1)=1, F(2)=1). */
    if (n >= 1)
        fib_arr[1] = (seq_t)1;
    if (n >= 2)
        fib_arr[2] = (seq_t)1;
    for (idx_t i = 3; i <= n; ++i)
        fib_arr[i] = fib_arr[i - 1] + fib_arr[i - 2];

    max_prepared = n;
}

/* The required OEIS sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                     /* undefined for n < 1 */

    prepare_upto(n);                         /* ensure σ and fib are ready */

    seq_t result = (seq_t)0;
    idx_t sqrt_n = isqrt_idx(n);

    for (idx_t d = 1; d <= sqrt_n; ++d) {
        if (n % d == 0) {                    /* d is a divisor of n */
            idx_t q = n / d;                 /* complementary divisor */
            result += sigma_arr[d] * fib_arr[q];
            if (d != q)                     /* avoid double counting square root */
                result += sigma_arr[q] * fib_arr[d];
        }
    }
    return result;
}
