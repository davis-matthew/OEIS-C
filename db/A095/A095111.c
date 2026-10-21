/*
 * A095111  One minus the parity of 1-fibits in Zeckendorf expansion
 *          A014417(n).
 * https://oeis.org/A095111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 minus the parity of the number of 1‑bits in the
 *            Zeckendorf (non‑adjacent Fibonacci) representation of n.
 *            Recurrence: a'(1) = 1, for n ≥ 2 with F(k) < n ≤ F(k+1) set
 *            a'(n) = 1 – a'(n – F(k)); then a(n) = a'(n+1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static tables for the recurrence values a'(i) and for Fibonacci numbers.
   a'(i) is stored 1‑indexed:  a_prime[1] = a'(1).  */

static seq_t   *a_prime    = NULL;
static size_t   a_prime_cap = 0;
static size_t   a_prime_len = 0;      /* maximal index computed so far */

static idx_t   *fib_arr    = NULL;
static size_t   fib_cap    = 0;
static size_t   fib_len    = 0;        /* number of Fibonacci values stored */

/* Index ptr such that fib_arr[fib_fptr] < i ≤ fib_arr[fib_fptr+1] for
   the current i when computing a'(i).  */
static idx_t   fib_fptr = 1;

/* Grow a generic integer array to hold at least 'need' elements. */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need)
    {
        size_t newcap = (*cap) ? (*cap) * 2 : 8;
        if (newcap < need) newcap = need;
        *arr = (seq_t*)realloc(*arr, newcap * sizeof(seq_t));
        *cap = newcap;
    }
}

/* Grow the Fibonacci array to contain values up to and exceeding 'upto'. */
static void ensure_fib(idx_t upto)
{
    if (fib_arr == NULL)
    {
        fib_cap = 64;
        fib_arr = (idx_t*)malloc(fib_cap * sizeof(idx_t));
        fib_arr[0] = 0;
        fib_arr[1] = 1;
        fib_arr[2] = 1;   /* F(1)=1, F(2)=1 */
        fib_len = 3;
    }

    /* Expand capacity if necessary. */
    while (fib_cap <= upto && fib_cap < 1000000)
    {
        size_t newcap = fib_cap * 2;
        if (newcap < fib_cap) newcap = fib_cap + 1;
        fib_arr = (idx_t*)realloc(fib_arr, newcap * sizeof(idx_t));
        fib_cap = newcap;
    }

    /* Append Fibonacci values until we exceed 'upto'. */
    while (fib_len <= upto)
    {
        fib_arr[fib_len] = fib_arr[fib_len-1] + fib_arr[fib_len-2];
        fib_len++;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t m = n + 1;                     /* we need a'(m) where m = n+1 */

    /* Ensure internal tables are initialized. */
    if (a_prime == NULL)
        grow_seq(&a_prime, &a_prime_cap, 4);        /* arbitrary start size */

    if (fib_arr == NULL)
        ensure_fib(2);   /* minimal initialization; will grow as needed */

    /* Make sure Fibonacci table contains a value > m. */
    ensure_fib(m);

    /* Allocate enough space for a'(1)…a'(m). */
    grow_seq(&a_prime, &a_prime_cap, m + 1);

    /* Initialize base case if not already done. */
    if (a_prime_len == 0)
    {
        a_prime[1] = (seq_t)1;   /* a'(1)=1 */
        a_prime_len = 1;
        fib_fptr = 1;            /* reset pointer for consistency */
    }

    /* Compute a' values up to index m if necessary. */
    while (a_prime_len < m)
    {
        idx_t i = a_prime_len + 1;          /* next index to compute */

        /* Advance fib_fptr so that fib_arr[fib_fptr] < i ≤ fib_arr[fib_fptr+1] */
        while (fib_fptr + 1 < fib_len && fib_arr[fib_fptr + 1] < i)
            fib_fptr++;

        /* a'(i) = 1 – a'(i – fib_arr[fib_fptr])  */
        seq_t val = (seq_t)1 - a_prime[i - fib_arr[fib_fptr]];
        a_prime[i] = val;
        a_prime_len = i;
    }

    /* a(n) = a'(n+1) = a_prime[m] */
    return a_prime[m];
}
