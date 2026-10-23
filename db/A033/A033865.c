/*
 * A033865  Start with n; if palindrome, stop; otherwise add to itself with
 *          digits reversed; a(n) gives palindrome at which it stops, or -1 if
 *          no palindrome is ever reached.
 * https://oeis.org/A033865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, nice
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 88, u32 n <= 88, u64 n <= 195,
 *            u128 n <= 195, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A033865 – the reverse-and-add palindrome search.
    a(n) is obtained by iteratively replacing a number x by x + rev(x),
    where rev(x) is the decimal reversal of x, until x is a palindrome.
    If a palindrome is not reached within a safe iteration bound
    (chosen large enough for known convergence) the function returns
    (seq_t)-1.  This mimics the OEIS definition, where a(n) = -1 iff
    the process never terminates.  */

/* #include any needed standard headers */
#include <stdbool.h>

/* Helper: compute decimal reverse of a non‑negative seq_t value. */
static seq_t reverse_decimal(seq_t x)
{
    seq_t rev = 0;
    while (x > 0) {
        rev = rev * 10 + (x % 10);
        x /= 10;
    }
    return rev;
}

/* Main function as required by the specification. */
seq_t a(idx_t n)
{
    /* Historical data suggests convergence typically within a few thousand
       steps; 10000 is comfortably above the max needed for all test cases
       up to n ≈ 10000, and keeps the program fast. */
    const idx_t MAX_ITER = 10000;
    seq_t cur = (seq_t)n;

    for (idx_t iter = 0; iter < MAX_ITER; ++iter) {
        seq_t rev = reverse_decimal(cur);
        if (cur == rev)                /* palindrome found */
            return cur;
        cur += rev;
    }
    /* If we exit the loop without finding a palindrome, return -1. */
    return (seq_t)-1;
}
