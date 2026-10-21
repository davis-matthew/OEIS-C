/*
 * A087269  Nonprime solutions to gcd(x, pi(x)) = gcd(x, A000720(x)) = 1.
 * https://oeis.org/A087269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth nonprime integer x such that gcd(x,π(x)) = 1
 *            (π(x) is the prime-counting function; a(1) = 1)
 * Range:     u8 n <= 95, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/is_prime.h"

static seq_t *seq = NULL;         /* array of found terms */
static size_t seq_len = 0;          /* number of terms stored */
static size_t seq_cap = 0;          /* allocated capacity */
static idx_t checked = 1;           /* next integer to test */
static seq_t pi = 0;                /* π(x) for the last tested integer */

/* Append a value to the sequence, expanding storage if necessary. */
static void push_term(idx_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap == 0 ? 16 : seq_cap * 2;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
    seq[seq_len++] = (seq_t)val;
}

/* Compute the nth term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) /* undefined for n=0, return 0 as safety */
        return (seq_t)0;

    /* Generate terms until we have at least n of them. */
    while (seq_len < (size_t)n) {
        idx_t x = checked;
        bool is_p = is_prime(x);  /* uses the provided is_prime tool */

        if (is_p) {
            /* prime: increase π(x) */
            ++pi;
        } else {
            /* nonprime (including 1): test gcd condition */
            if (gcd((seq_t)x, pi) == (seq_t)1)
                push_term(x);
        }
        ++checked;
    }

    return seq[n - 1];
}
