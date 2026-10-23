/*
 * A193957  Triangular array: the fusion of (p(n,x)) by (q(n,x)), where
 *          p(n,x)=sum{F(k+1)*x^(n-k) : 0<=k<=n}, where F=A000045 (Fibonacci
 *          numbers), and q(n,x)=sum{(k+1)(n+1)*x^(n-k) : 0<=k<=n}.
 * https://oeis.org/A193957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the element at row r and position k of the triangular
 *            array obtained by fusing the polynomials p(n,x) = Σ_{j=0}^n
 *            F(j+1)·x^{n-j} and q(n,x) = Σ_{j=0}^n (j+1)(n+1)·x^{n-j}. In the
 *            array the element at row r (r≥0) and column k (0≤k≤r) is a_k·r −
 *            Σ_{i=0}^{k-1} a_i, where a_0 = 1, a_j = a_{j-...
 * Range:     u8 n <= 34, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Returns the n‑th term of OEIS A193957.
    idx_t   : unsigned 64‑bit index type supplied by the build.
    seq_t   : unsigned integer type (8,16,32,64 bit or arbitrary‑precision) supplied by the build. */

#include <stdlib.h>
#include "../utils/fib.h"

/* static storage for the sequences a_j and their prefix sums */
static seq_t *Aj = NULL;          /* a_j values  */
static seq_t *pref = NULL;        /* pref[j] = Σ_{i=0}^j a_i */
static size_t cap = 0;            /* allocated capacity */

/* enlarge the static arrays up to index need (inclusive) */
static void ensure_size(size_t need)
{
    if (need < cap)
        return;

    size_t newcap = cap ? cap * 2 : 16;
    while (newcap <= need)
        newcap <<= 1;

    Aj  = (seq_t *)realloc(Aj,  newcap * sizeof(seq_t));
    pref = (seq_t *)realloc(pref, newcap * sizeof(seq_t));

    /* fill the new entries */
    for (size_t i = cap; i < newcap; ++i)
    {
        if (i == 0)
        {
            Aj[i]   = (seq_t)1;
            pref[i] = Aj[i];
        }
        else
        {
            /* Af(i) = A{i-1} + F(i+1) */
            Aj[i] = Aj[i - 1] + fib((idx_t)(i + 1));
            pref[i] = pref[i - 1] + Aj[i];
        }
    }
    cap = newcap;
}

/* Main function implementing the sequence. */
seq_t a(idx_t n)
{
    /* find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2   */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* offset within the row (0‑based) */
    idx_t k = n - r * (r + 1) / 2;

    /* special case: the very first element of the triangle. */
    if (r == 0)
        return (seq_t)1;

    /* make sure that a_k and the prefix sums exist */
    ensure_size((size_t)k);

    seq_t ak   = Aj[(size_t)k];
    seq_t sum  = k ? pref[(size_t)(k - 1)] : (seq_t)0;

    /* value = a_k * r - sum_{i=0}^{k-1} a_i */
    return ak * (seq_t)r - sum;
}
