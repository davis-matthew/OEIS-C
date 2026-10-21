/*
 * A223486  Lucas entry points: a(n) = least k such that n divides Lucas
 *          number L_k (=A000032(k), for k >= 0), or -1 if there is no such k.
 * https://oeis.org/A223486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   smallest k ≥ 0 such that n divides Lucas number L_k =
 *            A000032(k). The Lucas sequence is defined by L_0 = 2, L_1 = 1,
 *            L_{k+2} = L_{k+1} + L_k. If no such k exists, return -1
 * Range:     i8 n <= 205, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle n = 1 separately – 0 always divides L_0. */
    uint64_t mod = n;            /* n fits in 64‑bit unsigned. */
    if (mod == 1) return 0;      /* 1 divides L_0 = 2. */

    uint64_t start_prev = 2 % mod;   /* L_0 mod n */
    uint64_t start_curr = 1 % mod;   /* L_1 mod n */

    /* k = 0 case: */
    if (start_prev == 0) return 0;
    if (start_curr == 0) return 1;

    /* Run the recurrence modulo n, stopping if we find a zero
       or if we return to the starting pair (no further repetition can
       produce a zero). */
    uint64_t prev = start_prev;      /* L_{k-1} mod n */
    uint64_t curr = start_curr;      /* L_k     mod n */
    uint64_t k = 1;                  /* we are currently at L_1 */

    for (;;)
    {
        uint64_t next = (prev + curr) % mod;  /* L_{k+1} mod n */
        ++k;                                 /* next index is k+1 */

        if (next == 0)
            return (seq_t)k;                  /* we found the entry */

        prev = curr;
        curr = next;

        /* If the pair has cycled back to the start, no entry exists. */
        if (prev == start_prev && curr == start_curr)
            return (seq_t)-1;
    }
}
