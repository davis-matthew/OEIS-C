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
 * Helpers:   first_repeat_fib_odd_mod
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
#include "../utils/first_repeat_fib_odd_mod.h"

seq_t a(idx_t n) {
    /* OEIS offset starts at 1: a(1) is the first term.  For n = 0 we return 0. */
    if (n == 0) return (seq_t)0;
    return first_repeat_fib_odd_mod(n);
}
