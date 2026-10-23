/*
 * A130258  Partial sums of the 'upper' odd Fibonacci Inverse A130256.
 * https://oeis.org/A130258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*A130256(n) - Fib(2*A130256(n)-2) - 1 where A130256(n) =
 *            smallest m such that Fib(2m-1) ≥ n
 * Range:     u8 n <= 51, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We store Fib(2k-1) for k=1,2,… in a static cache so that
   each call can find m in O(log k) time and we recompute only the
   new Fibonacci numbers as needed.                     */

#include <stdlib.h>

static seq_t *odd_fibs = NULL;           /* Fib(1), Fib(3), Fib(5), …   */
static idx_t odd_fibs_count = 0;
static idx_t odd_fibs_cap   = 0;

/* Fast Fibonacci for small n (≤ ~128).  Returns Fib(n). */
static seq_t fib_n(idx_t n)
{
    seq_t a = (seq_t)0; /* F(0) */
    seq_t b = (seq_t)1; /* F(1) */
    idx_t i;
    for (i = 0; i < n; ++i) {
        seq_t tmp = a + b;
        a = b;
        b = tmp;
    }
    return a;
}

/* Ensure odd_fibs contains Fib(2k-1) for all k up to at least the
   smallest index m such that Fib(2m-1) ≥ max_n. */
static void init_odd_fibs(idx_t max_n)
{
    if (!odd_fibs) {
        odd_fibs_cap = 64;
        odd_fibs = (seq_t *)malloc(odd_fibs_cap * sizeof(seq_t));
        if (!odd_fibs) { odd_fibs_cap = 0; return; }
    }

    while (odd_fibs_count == 0 ||
           odd_fibs[odd_fibs_count - 1] < (seq_t)max_n) {

        idx_t m = odd_fibs_count + 1;            /* next odd-index count  */
        seq_t val = fib_n(2 * m - 1);             /* Fib(2m-1)            */

        if (odd_fibs_count == odd_fibs_cap) {
            odd_fibs_cap <<= 1;
            odd_fibs = (seq_t *)realloc(odd_fibs,
                                         odd_fibs_cap * sizeof(seq_t));
            if (!odd_fibs) { odd_fibs_cap = 0; return; }
        }
        odd_fibs[odd_fibs_count++] = val;
    }
}

/* Binary search over odd_fibs to find the smallest m such that
   Fib(2m-1) ≥ n.  m is always ≥ 1. */
static idx_t find_m(idx_t n)
{
    idx_t lo = 0, hi = odd_fibs_count - 1, ans = odd_fibs_count - 1;

    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (odd_fibs[mid] >= (seq_t)n) {
            ans = mid;
            if (mid == 0) break;
            hi  = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    return ans + 1;   /* convert 0‑based array index to 1‑based m     */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* a(0) is defined as 0 */

    init_odd_fibs(n);                       /* ensure cache up to n   */

    idx_t m = find_m(n);                    /* A130256(n)              */

    seq_t fn = fib_n(2 * m - 2);             /* Fib(2m-2)               */

    /* Compute a(n) = n*m - fn - 1   (no intermediate overflow for n≤10000) */
    seq_t res = (seq_t)n * (seq_t)m - fn - (seq_t)1;
    return res;
}
