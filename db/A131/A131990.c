/*
 * A131990  Consider the periodic sequence S(n) defined by: b(1)=2, b(i+1) =
 *          b(i) + n if i is prime, b(i+1) = gpf(b(i)) if i is not prime,
 *          where gpf means "greatest prime factor"; then a(n) is the length
 *          of the repeating period of the sequence.
 * https://oeis.org/A131990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   gpf, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the length of the repeating period of the sequence
   defined by b(1)=2 and,
   b(i+1) = b(i)+n  if  b(i) is prime,
   b(i+1) = gpf(b(i))  otherwise.
   a(n) is the size of the eventual cycle of this sequence.   */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/gpf.h"

/* Step function of the recurrence. */
static inline seq_t step(seq_t cur, seq_t N)
{
    return isPrimeGeneric(cur) ? cur + N : (seq_t)gpf((uint64_t)cur);
}

/* Public function required by the exerciser. */
seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;          /* cast index to seq_t once */
    seq_t tort = step((seq_t)2, N);
    seq_t hare = step(step((seq_t)2, N), N);

    /* Find first intersection of tortoise and hare (Floyd). */
    while (tort != hare) {
        tort = step(tort, N);
        hare = step(step(hare, N), N);
    }

    /* Compute the length λ of the cycle. */
    seq_t lambda = (seq_t)1;
    hare = step(tort, N);
    while (tort != hare) {
        hare = step(hare, N);
        ++lambda;
    }
    return lambda;
}
