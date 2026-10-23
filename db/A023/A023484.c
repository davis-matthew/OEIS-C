/*
 * A023484  a(n) = b(n) + d(n), where b(n) = (n-th Lucas number) and d(n) =
 *          (n-th non-Fibonacci number).
 * https://oeis.org/A023484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000032(n+1) + A001690(n+1)
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 45, u64 n <= 91, u128 n <= 183,
 *            big any n
 * Helpers:   non_fib_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/non_fib_k.h"
/* A000032 is the Lucas sequence: L0 = 2, L1 = 1, L(n) = L(n-1) + L(n-2)  */
/* A001690 is the n-th non‑Fibonacci number (offset 1)                           */

static seq_t lucas_value(idx_t k)
{
    /* return the k‑th Lucas number Lk (k >= 0) */
    if (k == 0) return (seq_t)2;
    if (k == 1) return (seq_t)1;
    seq_t prev = (seq_t)2;        /* L0 */
    seq_t curr = (seq_t)1;        /* L1 */
    for (idx_t i = 2; i <= k; ++i) {
        seq_t next = prev + curr;
        prev = curr;
        curr = next;
    }
    return curr;
}

seq_t a(idx_t n)
{
    idx_t k = n + 1;                    /* indices for Lucas and non‑Fib */
    seq_t luc = lucas_value(k);         /* L_{n+1} */
    seq_t nf  = non_fib_k(k);           /* d(n+1) = (n+1)-th non‑Fib number */
    return luc + nf;
}
