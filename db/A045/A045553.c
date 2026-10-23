/*
 * A045553  Numbers whose factorial has '12' as its final two digits before
 *          the trailing zeros.
 * https://oeis.org/A045553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer k such that the * factorial k!
 *            with all trailing zeros removed ends with the two * non‑zero
 *            digits 12
 * Range:     u8 n <= 14, u16 n <= 3313, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow2_mod100
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow2_mod100.h"

/* storage for sequence values */
static seq_t *seq_arr    = NULL;   /* array of found indices */
static size_t seq_cap    = 0;      /* allocated capacity */
static size_t seq_len    = 0;      /* number of terms stored */

/* state for incremental factorial calculation */
static idx_t cur_num       = 0;    /* last processed integer   */
static seq_t prod_mod100   = 1;    /* product of stripped factors (mod 100) */
static idx_t two_excess    = 0;    /* total 2s removed minus 5s removed */

/* grow array if needed */
static void ensure_seq_cap(size_t needed)
{
    if (seq_cap >= needed) return;
    while (seq_cap < needed) seq_cap = (seq_cap == 0) ? 1 : seq_cap * 2;
    seq_arr = (seq_t *)realloc(seq_arr, seq_cap * sizeof(seq_t));
}

/* main function */
seq_t a(idx_t n)
{
    /* first, compute terms until we have at least n of them */
    while (seq_len < n) {
        ++cur_num;                     /* next integer to test          */
        idx_t x = cur_num;

        /* strip factors of 2 and 5 */
        while ((x & 1) == 0) { /* x % 2 == 0 */
            x >>= 1;
            ++two_excess;
        }
        while (x % 5 == 0) {
            x /= 5;
            --two_excess;
        }

        /* Update product modulo 100 */
        prod_mod100 = (prod_mod100 * (seq_t)(x % 100)) % 100;

        /* Compute last two non‑zero digits of cur_num! */
        seq_t last_two = prod_mod100;
        if (two_excess > 0) {
            last_two = (last_two * pow2_mod100(two_excess)) % 100;
        }

        /* Record if the condition is satisfied */
        if (last_two == 12) {
            ensure_seq_cap(seq_len + 1);
            seq_arr[seq_len++] = (seq_t)cur_num;
        }
    }

    /* return the requested term (1‑based index) */
    return seq_arr[n - 1];
}
