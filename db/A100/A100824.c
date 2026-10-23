/*
 * A100824  Number of partitions of n with at most one odd part.
 * https://oeis.org/A100824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 68, u32 n <= 216, u64 n <= 746,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A100824: Number of partitions of n with at most one odd part.
    Formula:
        a(2*n)   = p(n)                      // partition number of n
        a(2*n+1) = Σ_{k=0}^{n} p(k)         // cumulative partition sum
    where p(m) is the classic partition function obtained via the
    generalized pentagonal recurrence.  The code keeps a static
    table of partition numbers `P` and their prefix sums `S`, computing
    new entries only as needed.  It works for all supported `seq_t`
    types, including arbitrary‑precision integer classes.  The maximum
    n that may be requested is bounded by 10000, as required by the
    problem.  */

/* Maximum n expected (see problem statement).  The table needs to track
   partition numbers up to n/2, so we use (MAX_N/2)+1 entries. */
#define MAX_N 10000
#define MAX_PART (MAX_N / 2 + 1)

#include <stdbool.h>   /* for bool */

static seq_t P[MAX_PART];   /* partition numbers p(k), k >= 0 */
static seq_t S[MAX_PART];   /* cumulative sums Σ_{i=0}^k p(i) */
static bool   initialized = false;
static idx_t  maxP = 0;      /* highest k for which P[k] / S[k] are valid */

/* initialise the tables */
static void init_tables(void)
{
    if (initialized)
        return;
    P[0] = 1;      /* p(0) = 1 */
    S[0] = 1;      /* Σ_{i=0}^{0} p(i) = 1 */
    maxP  = 0;
    initialized = true;
}

/* compute partition numbers and prefix sums up to new_max (inclusive) */
static void ensure_size(idx_t new_max)
{
    if (new_max <= maxP)   /* nothing to do */
        return;

    for (idx_t m = maxP + 1; m <= new_max; ++m) {
        seq_t val = 0;    /* zero of type seq_t */

        /* generalized pentagonal numbers: g(k)=k(3k-1)/2 , h(k)=k(3k+1)/2 */
        for (idx_t k = 1; ; ++k) {
            idx_t g = k * (3 * k - 1) / 2;
            if (g > m) break;

            seq_t term = P[m - g];
            if (k & 1)
                val += term;          /* sign + for odd k */
            else
                val -= term;          /* sign - for even k */

            idx_t h = k * (3 * k + 1) / 2;
            if (h > m) continue;
            term = P[m - h];
            if (k & 1)
                val += term;
            else
                val -= term;
        }

        P[m] = val;
        S[m] = S[m - 1] + val;
    }

    maxP = new_max;
}

/* export: seq_t a(idx_t n)  */
seq_t a(idx_t n)
{
    init_tables();

    if (n & 1U) {                 /* n is odd: a(n) = Σ_{k=0}^{(n-1)/2} p(k) */
        idx_t k = (n - 1) >> 1;
        ensure_size(k);
        return S[k];
    } else {                       /* n is even: a(n) = p(n/2) */
        idx_t k = n >> 1;
        ensure_size(k);
        return P[k];
    }
}
