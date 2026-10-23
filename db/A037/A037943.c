/*
 * A037943  Smallest Fibonacci number that has n as a factor, divided by n.
 * https://oeis.org/A037943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 29, u64 n <= 69, u128 n <= 133,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Compute a(n) for the OEIS sequence A037943.
 * a(n) = F(rank(n)) / n
 * where rank(n) is the smallest positive integer k with n | F(k)
 * (the “rank of appearance” of n in the Fibonacci sequence).
 *
 * The implementation finds the rank by generating Fibonacci numbers
 * modulo n until a zero residue is seen.  Then it computes the exact
 * Fibonacci number F(k) with the supplied big‑integer routine
 * fib(idx_t) and divides the result by n.  No tables or hard‑coded
 * values are used; the routine works for any n ≥ 1 and can be
 * called in arbitrary order.
 */

#include <stdint.h>
#include "../utils/fib.h"   /* provides seq_t fib(idx_t n) */

seq_t a(idx_t n)
{
    /* Handle the trivial case n == 0 (not part of the sequence) */
    if (n == 0)
        return 0;

    /* -------------------------------------------------------------
     * 1. Find the rank of appearance of n by iterating Fibonacci
     *    numbers modulo n until a zero residue occurs.
     * ------------------------------------------------------------- */
    uint64_t modn   = (uint64_t)n;   /* n fits into 64 bits */
    uint64_t prev   = 0;             /* F(0)   mod n */
    uint64_t curr   = 1 % modn;      /* F(1)   mod n */
    idx_t    index  = 1;             /* current Fibonacci index i (curr = F(i)) */

    /* For n == 1, curr already equals 0 (1 % 1 == 0), so the loop
       will skip.  For other n, we search until curr % n == 0. */
    while ((curr % modn) != 0) {
        uint64_t next = (prev + curr) % modn; /* F(i+1) mod n */
        prev = curr;
        curr = next;
        ++index;                /* now curr == F(index) */
    }
    /* 'index' now holds the smallest k with n | F(k) */

    /* -------------------------------------------------------------
     * 2. Compute the exact Fibonacci number F(index) using fib().
     * ------------------------------------------------------------- */
    seq_t fib_value = fib(index);

    /* -------------------------------------------------------------
     * 3. Divide by n (exact division guaranteed by definition).
     * ------------------------------------------------------------- */
    seq_t result = fib_value / (seq_t)n;

    return result;
}
