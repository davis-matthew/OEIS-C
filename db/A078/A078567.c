/*
 * A078567  Number of arithmetic subsequences of [1..n] with length > 1.
 * https://oeis.org/A078567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 165, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Sum_{i=1}^n (n-i) * d(i)
 * where d(i) is the number of positive divisors of i.
 * This is equivalent to the OEIS definition of A078567.
 */

#include <stdlib.h>

static seq_t *divcnt = NULL;   /* divisor counts for 1..max_n   */
static idx_t  max_n   = 0;     /* largest n for which divcnt is valid */

/* Ensure that divcnt[1..n] is correctly filled.  Simple recomputation
   is used whenever n exceeds the current maximum.  */
static void
ensure_divcnt(idx_t n)
{
    if (n <= max_n) return;

    /* Allocate or grow the array.  Cast the void* from realloc
       to seq_t* so that the code compiles in C++ as well. */
    seq_t *newp = (seq_t *)realloc(divcnt, (size_t)(n + 1) * sizeof(seq_t));
    if (!newp) abort();          /* allocation failure */
    divcnt = newp;

    /* Initialise all entries to zero.  The loop assigns 0 to each
       element; this works for integer types and for arbitrary‑precision
       classes that support assignment from 0. */
    for (idx_t i = 1; i <= n; ++i) divcnt[i] = 0;

    /* Classic divisor–count sieve.  */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i) {
            divcnt[j] += 1;
        }
    }

    max_n = n;
}

/* External linkage as requested.  */
extern seq_t a(idx_t n)
{
    if (n <= 1) return 0;   /* a(1) = 0 */

    /* Build divisor counts up to n.  */
    ensure_divcnt(n);

    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t term = (seq_t)(n - i) * divcnt[i];
        sum += term;
    }
    return sum;
}
