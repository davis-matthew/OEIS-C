/*
 * A225613  The largest n-digit number whose first k digits are divisible by
 *          the k-th prime for k = 1..n.
 * https://oeis.org/A225613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The largest n‑digit number whose first k digits are divisible by the k‑th prime.
   Construction: start with 0 and, for k = 1..n, append the largest digit d
   (9 … 0) such that the new number is divisible by the k‑th prime. */
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    seq_t cur = 0;

    for (idx_t k = 1; k <= n; ++k) {
        uint32_t pk = get_prime(k);          /* k‑th prime */
        seq_t   cur10 = cur * 10ULL;         /* current number × 10 */

        /* try digits 9..0, pick the first that satisfies divisibility */
        for (int d = 9; d >= 0; --d) {
            seq_t val = cur10 + d;
            if (val % pk == 0) {
                cur = val;
                break;
            }
        }
    }
    return cur;
}
