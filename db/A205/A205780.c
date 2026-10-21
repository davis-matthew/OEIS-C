/*
 * A205780  Least positive integer k such that n divides C(k)-C(j) for some j
 *          in [1,k-1], where C=A007598 (squared Fibonacci numbers).
 * https://oeis.org/A205780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A205780
   a(n) = least positive integer k such that
          there exists j with 1 ≤ j < k and n | (C(k) – C(j)),
          where C(k) = F(k)^2 (squared Fibonacci number).
   This is equivalent to the first repetition of the sequence
   C(k) mod n.  The function computes this by iterating the
   Fibonacci recurrence modulo n, storing squared residues,
   and returning the index of the first duplicate.  The
   algorithm is O(k) with k ≤ n+1, which is well below the
   2‑second limit for n up to ~10 000.  It works for any
   seq_t: ordinary unsigned integer types or arbitrary‑precision
   classes.  The only additional operation is a modulo with a
   big integer class; a cast to seq_t protects the intermediate
   product from overflow. */

#include <stdlib.h>
#include <stdbool.h>

/* Parameters
   n          – the divisor (idx_t is unsigned 64‑bit)
   seq_t      – the sequence element type (u8, u16, ... or big int) */

seq_t a(idx_t n)
{
    /* Undefined for n==0 – return 0 as a defensive value */
    if (n == 0)
        return (seq_t)0;

    /* Allocate a seen array for residues 0 .. n-1.
       n is at most a few thousand in the required tests. */
    size_t sz = (size_t)n;
    bool *seen = (bool *)calloc(sz, sizeof(bool));
    if (!seen)
        return (seq_t)0;   /* allocation failure */

    /* Initialise Fibonacci numbers mod n:
       F(1)=1, F(2)=1 (mod n).  We use idx_t for the residues. */
    idx_t f_prev = 1 % n;   /* F(i-1) mod n */
    idx_t f_cur  = 1 % n;   /* F(i)   mod n   , i starts at 1 */

    for (idx_t k = 1; ; ++k) {
        /* Compute C(k) = F(k)^2 mod n using big‑int multiplication
           where the intermediate product is cast to seq_t to avoid
           overflow.  The result is then cast back to idx_t. */
        idx_t c = (idx_t)(((seq_t)f_cur * f_cur) % (seq_t)n);

        if (seen[c]) {
            free(seen);
            return (seq_t)k;   /* first repetition found */
        }
        seen[c] = true;

        /* Advance Fibonacci: F(i+1) = F(i-1)+F(i) mod n */
        idx_t f_next = (f_prev + f_cur) % n;
        f_prev = f_cur;
        f_cur  = f_next;
    }
}
