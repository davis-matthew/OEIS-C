/*
 * A050615  Products of distinct terms of Fibonacci(2^(i+2)): a(n) =
 *          Product_{i=0..floor(log_2(n+1))} F(2^(i+2))^bit(n,i).
 * https://oeis.org/A050615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i=0..⌊log₂(n+1)⌋} F(2^(i+2))^bit(n,i), where F(k) is
 *            the k‑th Fibonacci number (F(0)=0, F(1)=1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   fib, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#include "../utils/fib.h"     /* seq_t fib(idx_t n) */
#include "../utils/seq_mul.h" /* seq_t seq_mul(seq_t a, seq_t b) */

/* Computes the k‑th term of the OEIS sequence A050615. */
seq_t a(idx_t n)
{
    seq_t prod = (seq_t)1;          /* product starts at 1 */
    idx_t bit  = 0;                 /* current bit position */

    while (n != 0) {
        if (n & 1ULL) {             /* bit(i) == 1? */
            idx_t fib_idx = ((idx_t)1ULL) << (bit + 2); /* 2^(i+2) */
            seq_t f = fib(fib_idx);
            prod = seq_mul(prod, f);
        }
        n >>= 1;
        ++bit;
    }
    return prod;
}
