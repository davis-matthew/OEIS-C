/*
 * A067526  Numbers n such that n - 2^k is a prime or 1 for all k satisfying 0
 *          < k, 2^k < n.
 * https://oeis.org/A067526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   a(n) is the nth positive integer n such that for every k with 0
 *            < k and 2^k < n, the number n-2^k is either 1 or a prime
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   append_candidate, is_prime_big, pow2_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/append_candidate.h"
#include "../utils/pow2_seq_t.h"

/* Return the n-th term of OEIS sequence A067526.  */
seq_t a(idx_t n)
{
    /* Static state to allow non‑monotonic calls.  */
    static seq_t *seq  = NULL;      /* array of found terms   */
    static size_t seq_len = 0;      /* number of stored terms */
    static size_t seq_cap = 0;      /* allocated capacity    */
    static seq_t last_checked = (seq_t)2;  /* last n that was examined */

    /* Allocate the array on first use.  */
    if (seq == NULL) {
        seq_cap = 16;
        seq = (seq_t *)malloc(sizeof(seq_t) * seq_cap);
        if (seq == NULL) return (seq_t)0;  /* allocation failure */
    }

    /* Generate terms until we have at least n of them.  */
    while (seq_len < n) {
        seq_t candidate = last_checked + 1;
        last_checked   = candidate;

        bool good = true;
        /* Test all powers of two less than candidate.  */
        for (int k = 1; ; ++k) {
            seq_t p2 = pow2_seq_t(k);
            if (!(p2 < candidate))       /* stop when 2^k >= candidate */
                break;

            seq_t diff = candidate - p2;
            if (diff != (seq_t)1) {       /* 1 is always acceptable */
                if (!is_prime_big(diff)) { /* otherwise must be prime */
                    good = false;
                    break;
                }
            }
        }

        if (good) {
            /* Store the new term.  */
            append_candidate(&seq, &seq_len, &seq_cap, candidate);
        }
    }

    return seq[n - 1];  /* a(1) is the first element stored.  */
}
