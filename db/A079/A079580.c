/*
 * A079580  a(n)= Sum_{d divides n} a(abs(n/d-d)).
 * https://oeis.org/A079580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{d|n} a(|n/d - d|), a(0) = 1
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoises all values up to the greatest n
   requested so far.  The recursion terminates because for every
   divisor d of n we have |n/d - d| < n, thus the function only
   refers to strictly smaller indices.  The array is resized as
   needed, unlimited by the arbitrary‑precision build. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* seq_t and idx_t are defined by the build.  We only need
   stdint types for helpers. */
static seq_t *memo = NULL;
static idx_t memo_size = 0;   /* number of valid indices (>=0) */

/* Ensure memo array can hold indices up to `n` inclusive. */
static void ensure_capacity(idx_t n)
{
    if (n + 1 <= memo_size)
        return;

    idx_t new_size = n + 1;
    seq_t *new_mem = (seq_t *)realloc(memo, new_size * sizeof(seq_t));
    if (!new_mem) {
        /* Out of memory; abort gracefully. */
        abort();
    }
    /* Initialise the new slots to 0 (meaning "unset") */
    memset(new_mem + memo_size, 0, (new_size - memo_size) * sizeof(seq_t));
    memo = new_mem;
    memo_size = new_size;
}

/* Recursive helper with memoisation. */
static seq_t compute_a(idx_t n)
{
    ensure_capacity(n);
    if (memo[n] != 0 || n == 0) {  /* a(0)=1, other values >0 */
        if (n == 0)
            memo[0] = (seq_t)1;   /* initialise a(0) only once */
        return memo[n];
    }

    seq_t sum = (seq_t)0;
    idx_t i;
    for (i = (idx_t)1; i <= n / i; ++i) {
        if (n % i != 0)
            continue;

        /* divisor d1 = i */
        idx_t d1 = i;
        idx_t nd1 = n / d1;
        idx_t sub1 = (nd1 > d1) ? (nd1 - d1) : (d1 - nd1);
        sum += compute_a(sub1);

        /* divisor d2 = n/i, if distinct */
        if (i != nd1) {
            idx_t d2 = nd1;
            idx_t nd2 = n / d2;   /* equals i */
            idx_t sub2 = (nd2 > d2) ? (nd2 - d2) : (d2 - nd2);
            sum += compute_a(sub2);
        }
    }

    memo[n] = sum;
    return sum;
}

seq_t a(idx_t n) {
    return compute_a(n);
}
