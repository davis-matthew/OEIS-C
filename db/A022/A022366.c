/*
 * A022366  Fibonacci sequence beginning 0, 32.
 * https://oeis.org/A022366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 32*A000045(n) (Fibonacci sequence with F0=0, F1=1)
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 40, u64 n <= 86, u128 n <= 178,
 *            big any n
 * Helpers:   fib_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib_n.h"

seq_t a(idx_t n)
{
    return fib_n(n) * (seq_t)32;
}
