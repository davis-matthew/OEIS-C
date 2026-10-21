/*
 * A080301  Local ranking function for totally balanced binary sequences: if
 *          n's binary expansion is totally balanced (A080116(n)=1), then a(n)
 *          is its zero-based position among A000108((A000523(n)+1)/2)
 *          lexicographically ordered totally balanced binary sequences of the
 *          same width, otherwise -1.
 * https://oeis.org/A080301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = rank of n among all totally balanced (Dyck) binary *
 *            sequences of its length, 0‑based; -1 if n is not totally
 *            balanced. * The ranking algorithm is the standard Catalan
 *            ranking procedure * (CAGES book algorithm 3.23): for each ‘1’ in
 *            the word, add the * number of Dyck completions obtainable...
 * Range:     i8 n <= 60, i16 n <= 60, i32 n <= 60, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binom.h"

/* Count the number of Dyck completions of length rem starting from
 * balance 'b' (ones minus zeros).  The path must never go below zero
 * and must end at zero after rem steps.  Uses the ballot formula:
 *  count = binom(rem, (rem-b)/2) - binom(rem, (rem-b)/2-1) */
static inline seq_t
count_completions(idx_t b, idx_t rem)
{
    if (b > rem) return (seq_t)0;
    /* (rem - b) must be even for an integer number of up steps. */
    if (((rem - b) & 1ULL) != 0) return (seq_t)0;

    idx_t u = (rem - b) >> 1;          /* number of up steps (+1) */
    seq_t total   = binom(rem, u);      /* all paths of that shape    */
    seq_t reflected = 0;
    if (u > 0)                         /* paths that touch -1 level */
        reflected = binom(rem, u - 1);
    return total - reflected;
}

seq_t a(idx_t n)
{
    if (n == 0)                         /* empty Dyck word */
        return (seq_t)0;

    /* Determine binary length without leading zeros. */
    idx_t L = 0;
    idx_t tmp = n;
    while (tmp) { tmp >>= 1; ++L; }

    if (L & 1U)               /* odd length cannot be Dyck */
        return (seq_t)-1;

    /* First check that the binary string is a Dyck word. */
    idx_t balance = 0;
    for (idx_t i = L; i > 0; --i) {
        bool bit = (n >> (i - 1)) & 1ULL;
        balance += bit ? 1 : -1;
        if (balance > i - 1) return (seq_t)-1;   /* cannot close enough */
        if (balance < 0)    return (seq_t)-1;
    }
    if (balance != 0) return (seq_t)-1;           /* not balanced */

    /* Compute lexicographic rank. */
    seq_t rank = 0;
    balance = 0;
    for (idx_t i = L; i > 0; --i) {
        bool bit = (n >> (i - 1)) & 1ULL;

        if (bit) {                    /* consider putting 0 instead */
            idx_t new_balance = balance - 1;
            if (new_balance >= 0) {
                idx_t rem = i - 1;    /* remaining bits after this */
                rank += count_completions(new_balance, rem);
            }
            balance += 1;
        } else {
            balance -= 1;
        }
    }
    return rank;
}
