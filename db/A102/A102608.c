/*
 * A102608  Numbers L such that 1LM1 is prime and M=L+1.
 * https://oeis.org/A102608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-03)
 * Helpers:   is_prime_seq, mem, num_digits_idx_to_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute the n-th term of sequence A102608.
 *  The sequence consists of integers L such that the decimal number
 *  1LM1 (where M=L+1) is prime.  This implementation scans L
 *  sequentially, tests primeness of 1LM1, and stores qualifying L
 *  values in a static array that grows as needed.  Indexing follows
 *  OEIS: a(1) is the first listed term. */
#include <stdbool.h>
#include <stdlib.h>

#include <stdint.h>
#include "../utils/is_prime_seq.h"
#include "../utils/num_digits_idx_to_idx.h"
#include "../utils/mem.h"

/* return 10^exp as seq_t */
static seq_t pow10_seq(idx_t exp)
{
    seq_t res = (seq_t)1;
    while (exp != 0) {
        res *= 10;
        --exp;
    }
    return res;
}

/* Static storage for generated terms and state */
static seq_t *terms = NULL;
static idx_t terms_size = 0;   /* number of terms currently stored */
static idx_t terms_cap  = 0;   /* current capacity of the terms array */
static idx_t next_L     = 1;   /* next integer L to test */

/* External function requested by the specification */
seq_t a(idx_t n)
{
    /* Ensure we have at least n terms stored */
    while (terms_size < n) {
        /* Compute 1LM1 for current L */
        idx_t lenL = num_digits_idx_to_idx(next_L);
        idx_t lenM = num_digits_idx_to_idx(next_L + 1);

        /* 1 * 10^(lenL + lenM + 1)  */
        seq_t nnum = pow10_seq(lenL + lenM + 1);

        /* L * 10^(lenM + 1) */
        nnum += (seq_t)next_L * pow10_seq(lenM + 1);

        /* (L+1) * 10 + 1 */
        nnum += (seq_t)(next_L + 1) * (seq_t)10 + (seq_t)1;

        /* Test primality */
        if (is_prime_seq(nnum)) {
            /* Resize the array if necessary */
            if (terms_size == terms_cap) {
                idx_t new_cap = terms_cap ? terms_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, terms, new_cap);
                if (!tmp) {
                    /* Out of memory – abort */
                    exit(EXIT_FAILURE);
                }
                terms = tmp;
                terms_cap = new_cap;
            }
            /* Store the qualifying L */
            terms[terms_size++] = (seq_t)next_L;
        }
        /* Advance to the next L */
        ++next_L;
    }

    /* Return the requested term (1-based indexing) */
    return terms[n - 1];
}
