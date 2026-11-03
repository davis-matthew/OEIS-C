/*
 * A205443  The index j<k such that n divides s(k)-s(j) for some j, where
 *          s(j)=Fibonacci(2j-1), and k is the least index for which such a j
 *          exists.
 * https://oeis.org/A205443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A205443:  a(n) is the earliest index j<k such that n divides s(k)-s(j),
    where s(j)=Fibonacci(2j-1).  This is equivalent to the first repeat
    of a residue in the sequence of Fibonacci numbers taken at odd indices
    modulo n.  The algorithm below computes this in O(n) time and
    O(n) memory for each call of a(n).  It works for any idx_t, seq_t
    defined by the build and returns the correct value for all n≥1. */

#include <stdlib.h>
#include <stddef.h>

/* helper: compute Fib(2j-1) modulo mod for successive j, detecting first repeat */
static seq_t first_repeat_fib_odd_mod(idx_t mod) {
    /* Special case n = 1: first residue repeats immediately. */
    if (mod == 1) return (seq_t)1;

    /* Allocate array "first" that remembers the first index j of each
       residue.  Index 0..mod-1.  Initialise with 0 meaning “not seen yet”. */
    idx_t *first = (idx_t *)calloc((size_t)mod, sizeof(idx_t));
    if (!first) return (seq_t)0;  /* allocation failure – not expected for mod ≤ 10000 */

    /* Fibonacci recurrence: F(0)=0, F(1)=1.  We only need the values
       at odd indices, i.e. when the global Fibonacci index i is odd. */
    idx_t f0 = 0;                     /* F(i-1) */
    idx_t f1 = 1 % mod;                /* F(i)   (i starts at 1) */
    idx_t i   = 1;                     /* current Fibonacci index */
    idx_t j   = 1;                     /* current odd-index counter (for s(j)) */

    /* Record first occurrence of the residue for s(1)=F(1). */
    first[f1] = j;

    for (;;) {
        /* Next Fibonacci number modulo mod: F(i+1) = F(i-1)+F(i) mod mod */
        idx_t f2 = f0 + f1;
        if (f2 >= mod) f2 -= mod;      /* f0,f1 < mod → f2 < 2*mod */
        f0 = f1;
        f1 = f2;
        ++i;

        /* When i is odd we have a new term of s(j) = F(2j-1). */
        if (i & 1U) {
            ++j;                        /* move to new index j */
            if (first[f1] != 0) {       /* residue already seen → repeat found */
                seq_t result = (seq_t)first[f1];
                free(first);
                return result;
            }
            first[f1] = j;              /* remember first occurrence of this residue */
        }
    }
}

seq_t a(idx_t n) {
    /* OEIS offset starts at 1: a(1) is the first term.  For n = 0 we return 0. */
    if (n == 0) return (seq_t)0;
    return first_repeat_fib_odd_mod(n);
}
