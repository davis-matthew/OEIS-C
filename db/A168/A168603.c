/*
 * A168603  G.f. satisfies: A(x) = 1 + x*A(x)^2*A(2x)^2.
 * https://oeis.org/A168603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A168603:  G.f. satisfies A(x) = 1 + x*A(x)^2*A(2x)^2.
   Recurrence:  a(0)=1,
                a(n)=∑_{i=0}^{n-1} b(i)*b(n-1-i)*2^(n-1-i),
   where b(k)=∑_{p+q=k} a(p)*a(q).  b is updated incrementally. */
#include <stdlib.h>
#include <stdbool.h>

/* Sequence storage (static cache) */
static seq_t *a_arr   = NULL;   /* a[i]            */
static seq_t *b_arr   = NULL;   /* b[i]            */
static seq_t *pow2_arr= NULL;   /* 2^i             */
static idx_t   cap_a   = 0;      /* length of a_arr   */
static idx_t   cap_b   = 0;      /* length of b_arr   */
static idx_t   cap_pow= 0;      /* length of pow2_arr*/
static idx_t   last_computed = 0;/* highest n with a[i] known */
static bool    initialized = false;

/* Ensure a_arr has room for indices up to n */
static void ensure_a(idx_t n)
{
    if (n < cap_a) return;
    size_t new_cap = (size_t)n + 1;
    seq_t *new_a = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
    if (!new_a) abort();
    a_arr = new_a;
    for (idx_t i = cap_a; i < new_cap; ++i) a_arr[i] = (seq_t)0;
    cap_a = new_cap;
}

/* Ensure b_arr has room for indices up to n */
static void ensure_b(idx_t n)
{
    if (n < cap_b) return;
    size_t new_cap = (size_t)n + 1;
    seq_t *new_b = (seq_t *)realloc(b_arr, new_cap * sizeof(seq_t));
    if (!new_b) abort();
    b_arr = new_b;
    for (idx_t i = cap_b; i < new_cap; ++i) b_arr[i] = (seq_t)0;
    cap_b = new_cap;
}

/* Ensure pow2_arr has room for indices up to n and fill new entries */
static void ensure_pow2(idx_t n)
{
    if (n < cap_pow) return;
    size_t new_cap = (size_t)n + 1;
    seq_t *new_pow = (seq_t *)realloc(pow2_arr, new_cap * sizeof(seq_t));
    if (!new_pow) abort();
    pow2_arr = new_pow;
    if (cap_pow == 0) pow2_arr[0] = (seq_t)1;
    for (idx_t i = cap_pow; i < new_cap; ++i) {
        pow2_arr[i] = pow2_arr[i-1] * (seq_t)2;
    }
    cap_pow = new_cap;
}

/* Public routine: returns a(n) */
seq_t a(idx_t n)
{
    /* Initialize cache on first call */
    if (!initialized) {
        ensure_a(0);
        ensure_b(0);
        ensure_pow2(0);
        a_arr[0] = (seq_t)1;
        b_arr[0] = (seq_t)1;        /* b(0)=a0*a0 */
        pow2_arr[0] = (seq_t)1;
        last_computed = 0;
        initialized = true;
    }

    if (n < cap_a) {
        if (n > last_computed) {
            /* Should not happen: we must compute missing values */
            abort();
        }
        return a_arr[n];
    }

    /* Make sure all arrays are large enough */
    ensure_a(n);
    ensure_b(2*n);          /* b may be accessed up to 2*n during updates */
    ensure_pow2(n);

    /* Compute missing terms incrementally */
    for (idx_t t = last_computed + 1; t <= n; ++t) {
        /* a(t) = Σ_{i=0}^{t-1} b(i)*b(t-1-i)*2^(t-1-i) */
        seq_t sum = (seq_t)0;
        for (idx_t i = 0; i < t; ++i) {
            seq_t prod = b_arr[i] * b_arr[t-1-i];
            prod = prod * pow2_arr[t-1-i];
            sum += prod;
        }
        a_arr[t] = sum;

        /* Update b for indices k = t + s, 0 ≤ s ≤ t */
        for (idx_t s = 0; s <= t; ++s) {
            idx_t k = t + s;
            seq_t prod = a_arr[t] * a_arr[s];
            if (s == t)
                b_arr[k] += prod;            /* (t,t) counted once */
            else
                b_arr[k] += prod * (seq_t)2; /* (t,s) and (s,t) */
        }
    }

    last_computed = n;
    return a_arr[n];
}
