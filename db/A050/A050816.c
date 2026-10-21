/*
 * A050816  Fibonacci(k) ending with digits of its index number k.
 * https://oeis.org/A050816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A050816: a(n) is the nth Fibonacci number F(k) where F(k) ends with the
 * decimal digits of k. The indices k are taken in increasing order. */
#include <stdbool.h>
#include "../utils/digits.h"

/* Pre-computed powers of 10 (seq_t).  Adjust MAX_DIGITS if larger indices are
 * required.  The values remain valid for `int` indices up to 10¹⁴. */
#define MAX_DIGITS 15
static seq_t pow10[MAX_DIGITS + 1];
static bool pow10_init = false;

/* Storage for the sequence values.  The array size should comfortably
 * exceed 10 000 terms; 30 000 is chosen as a safe upper bound for typical
 * problems.  The storage is static so it is reused across calls. */
#define MAX_TERMS 30000
static seq_t seq_a[MAX_TERMS];
static idx_t a_count = 0;

/* Current state of Fibonacci generation.  These values are maintained
 * between calls to avoid recomputing the whole sequence each time. */
static idx_t fib_i = 1;      /* last Fibonacci index computed (currently 1) */
static seq_t fib_prev2 = 0;  /* F(0) */
static seq_t fib_prev1 = 1;  /* F(1) */

/* Initialise storage and the powers of 10 once. */
static void seq_init(void)
{
    if (pow10_init) return;
    pow10_init = true;
    pow10[0] = (seq_t)1;
    for (int i = 1; i <= MAX_DIGITS; ++i)
        pow10[i] = pow10[i - 1] * (seq_t)10;

    /* Process index 0 and 1 as the first terms. */
    seq_t f0 = fib_prev2;
    seq_t f1 = fib_prev1;
    if ((f0 % pow10[1]) == (seq_t)0)
        seq_a[a_count++] = f0;
    if ((f1 % pow10[1]) == (seq_t)1)
        seq_a[a_count++] = f1;
}

/* Main function required by the specification. */
seq_t a(idx_t n)
{
    seq_init();

    /* Generate terms until we have at least n+1 terms. */
    while (a_count <= n && fib_i < MAX_TERMS) {
        /* Compute next Fibonacci number. */
        seq_t fib_next = fib_prev1 + fib_prev2;
        ++fib_i;

        /* Determine number of decimal digits of the index fib_i. */
        int d = digits(fib_i);
        /* Use the pre-computed power of 10 for modulo comparison. */
        seq_t mod = fib_next % pow10[d];
        if (mod == (seq_t)fib_i) {
            if (a_count < MAX_TERMS)
                seq_a[a_count++] = fib_next;
        }

        /* Advance Fibonacci state. */
        fib_prev2 = fib_prev1;
        fib_prev1 = fib_next;
    }

    /* If n is beyond what we can compute, return 0 as a safe fallback. */
    if (n < a_count)
        return seq_a[n];
    return (seq_t)0;
}
