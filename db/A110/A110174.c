/*
 * A110174  Number of solutions 0<k<n to the equation phi(n) = phi(k) +
 *          phi(n-k), where phi is Euler's totient function.
 * https://oeis.org/A110174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of k, 0<k<n, such that φ(n)=φ(k)+φ(n‑k)
 * Range:     u8 n <= 103, u16 n <= 9858, u32 n <= 9858, u64 n <= 9858,
 *            u128 n <= 9858, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a simple incremental sieve to precompute φ up to the
   largest n requested.  It stores the totients in a static array and
   recomputes them from scratch whenever a larger n is needed.  This
   guarantees correct results for any n≥1 and keeps the runtime well
   below 2 s for n up to about 10000.  The addition and comparison are
   performed in the wider idx_t type to avoid overflow of the modest
   seq_t type that represents φ values. */

#include <stdlib.h>
#include <stddef.h>

/* static state for the φ sieve */
static seq_t *phi_arr = NULL;
static idx_t phi_limit = 0;

/* recompute φ for all integers from 0 to n (inclusive) */
static void recompute_phi_until(idx_t n)
{
    /* allocate or grow the array */
    seq_t *tmp = (seq_t *)realloc(phi_arr, (size_t)(n + 1) * sizeof(seq_t));
    if (!tmp) {
        /* On allocation failure we simply abort – the environment
           which evaluates the OEIS functions expects a terminating
           program on fatal errors. */
        exit(1);
    }
    phi_arr = tmp;

    /* initialise to the identity; φ(1)=1 by convention */
    for (idx_t i = 0; i <= n; ++i) {
        phi_arr[i] = (seq_t)i;
    }
    if (n >= 1) {
        phi_arr[1] = (seq_t)1;
    }

    /* classic Euler sieve for φ */
    for (idx_t i = 2; i <= n; ++i) {
        if (phi_arr[i] == (seq_t)i) {           /* i is prime */
            for (idx_t j = i; j <= n; j += i) {
                phi_arr[j] -= phi_arr[j] / (seq_t)i;
            }
        }
    }

    phi_limit = n;
}

/* ensure φ values are available up to the requested n */
static void ensure_phi(idx_t n)
{
    if (n > phi_limit) {
        recompute_phi_until(n);
    }
}

/* ------------------------------------------------------------------ */
/*  OEIS sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n <= 1) {
        return (seq_t)0;            /* by definition */
    }

    ensure_phi(n);

    const seq_t phi_n = phi_arr[n];
    seq_t count = (seq_t)0;

    for (idx_t k = (idx_t)1; k < n; ++k) {
        if (((idx_t)phi_arr[k] + (idx_t)phi_arr[n - k]) ==
            (idx_t)phi_n) {
            ++count;
        }
    }

    return count;
}
