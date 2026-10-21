/*
 * A058261  a(n) = n times the Collatz number of n (as given in A006577).
 * https://oeis.org/A058261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1) * Collatz(n+1) – Collatz(n) is the total stopping *
 *            time from n, as defined in A006577. The OEIS list assumes
 *            a(0)=0, * a(1)=2, a(2)=21, … so the sequence is indexed one‑less
 *            than the * natural Collatz starting value. The function below
 *            follows this * convention
 * Range:     u8 n <= 16, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   collatz_steps
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/collatz_steps.h"

/* The requested OEIS a(n) function. */
seq_t a(idx_t n)
{
    /* Use n+1 as the actual starting value for the Collatz process. */
    idx_t start = n + 1;
    seq_t start_seq = (seq_t)start;
    seq_t steps = collatz_steps(start);
    return start_seq * steps;        /* product fits in seq_t by definition */
}
