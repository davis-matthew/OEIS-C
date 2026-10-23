/*
 * A102496  Values of n for which the concatenation of the form 1nn1 (sequence
 *          A100846) are primes.
 * https://oeis.org/A102496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   digits, is_prime_seq, pow10_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A102496 – values of n for which the concatenation 1nn1
   is prime.  The function increments candidate n starting at 1,
   skips numbers with an odd number of decimal digits or with n ≡ 2 (mod 3)
   (properties of the terms), constructs the number 1nn1,
   tests it for primality and records those n that succeed.
   The sequence is stored in a static array; this is sufficient for
   indices up to at least 10 000. */
#include <stdbool.h>
#include "../utils/digits.h"
#include "../utils/pow10_seq.h"
#include "../utils/is_prime_seq.h"

#define MAX_TERMS 20000   /* enough for all required tests */

static seq_t terms[MAX_TERMS];
static idx_t term_count   = 0;   /* number of terms already found */
static idx_t last_checked = 0;   /* last candidate examined */

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) {               /* offset is 1; n=0 is undefined */
        return 0;
    }

    /* Ensure we have computed at least the n‑th term.  We keep the list
       in increasing order, so each new term is appended. */
    while (term_count < n && term_count < MAX_TERMS) {
        ++last_checked;         /* next candidate */

        /* Skip candidates that cannot satisfy the necessary conditions. */
        idx_t d = digits(last_checked);          /* number of decimal digits */
        if ((d & 1) || (last_checked % 3 == 2)) continue;

        /* Build the number 1nn1:
             1 * 10^(2d+1) + n * 10^(d+1) + n * 10 + 1 */
        seq_t cand   = (seq_t)last_checked;
        seq_t number = pow10_seq(2 * d + 1);
        number += cand * pow10_seq(d + 1);
        number += cand * (seq_t)10;
        number += (seq_t)1;

        if (is_prime_seq(number)) {   /* prime found */
            terms[term_count++] = cand;
        }
    }

    /* The array is guaranteed large enough for the requested index. */
    return terms[n - 1];
}
