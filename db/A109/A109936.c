/*
 * A109936  Composite numbers between largest n-digit prime and the smallest
 *          (n+1) digit prime.
 * https://oeis.org/A109936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the n-th composite number that lies strictly between the
 *            largest n‑digit prime and the smallest (n+1)-digit prime. The
 *            sequence enumerates all such composites in increasing order
 * Range:     u8 n <= 6, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   isprime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"       /* seq_t pow10(idx_t exp) */
#include "../utils/isprime_big.h" /* bool isprime_big(seq_t n) */

seq_t a(idx_t n)
{
    idx_t target = n;      /* 1‑based index in the composite list */
    idx_t cumulative = 0;  /* number of composites processed so far */

    for (idx_t len = 1; ; ++len) {
        /* 10^len gives the first number with len+1 digits */
        seq_t start_next_digit = pow10(len);                /* 10^len  */
        seq_t high_start = start_next_digit - 1; /* largest len‑digit number */

        /* Find the largest len‑digit prime P */
        seq_t P = high_start;
        while (!isprime_big(P)) {
            P -= 1;
        }

        /* Find the smallest (len+1)-digit prime Q */
        seq_t Q = start_next_digit;
        while (!isprime_big(Q)) {
            Q += 1;
        }

        /* Number of composites in this block: (Q - P - 1) */
        seq_t block_size_seq = Q - P - 1;
        idx_t block_size = (idx_t)block_size_seq; /* small enough to fit in idx_t */

        if (cumulative + block_size >= target) {
            idx_t offset = target - cumulative - 1; /* zero‑based offset within the block */
            return P + 1 + offset;                 /* first composite is P+1 */
        }

        cumulative += block_size;
    }
}
