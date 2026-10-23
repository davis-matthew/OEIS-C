/*
 * A108787  Binary numbers such that the first differences give the same
 *          sequence written as a string of individual digits.
 * https://oeis.org/A108787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 56, u32 n <= 1927, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   decimal_of_binary
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence:  a(1)=1;
   for n>1, a(n)=a(n-1)+d_{n-1}
   where d_k is the k‑th bit (0 or 1) in the concatenation of the binary
   representations of the terms a(1),a(2),…;
   the function returns the decimal integer whose decimal digits equal that
   binary representation. */
#include <stdlib.h>
#include "../utils/decimal_of_binary.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1 */

    /* Persistent storage across calls */
    static seq_t *bin_vals  = NULL;   /* binary integer values */
    static seq_t *dec_vals  = NULL;   /* decimal-by-bits values */
    static size_t vals_cap  = 0;
    static idx_t  vals_len  = 0;

    static char  *bits      = NULL;   /* concatenated bits as '0'/'1' */
    static size_t bits_cap  = 0;
    static size_t bits_len  = 0;
    static size_t bits_pos  = 0;      /* next bit to use for a difference */

    /* Initialise on first call */
    if (vals_len == 0) {
        vals_cap = 16;
        vals_len = 1;
        bin_vals = (seq_t *)malloc(vals_cap * sizeof(seq_t));
        dec_vals = (seq_t *)malloc(vals_cap * sizeof(seq_t));
        if (!bin_vals || !dec_vals) abort();

        bin_vals[0] = (seq_t)1;
        dec_vals[0] = (seq_t)1;          /* decimal representation of "1" */

        bits_cap = 16;
        bits_len = 1;
        bits_pos = 0;
        bits = (char *)malloc(bits_cap * sizeof(char));
        if (!bits) abort();
        bits[0] = '1';    /* binary digits of a(1) */
    }

    /* Ensure we computed up to n */
    idx_t target = n;
    for (idx_t cur = vals_len; cur < target; ++cur) {
        /* Use the current bit as the difference (0 or 1) */
        seq_t diff = (seq_t)(bits[bits_pos] - '0');

        /* Compute next binary value and its decimal representation */
        seq_t next_bin = bin_vals[cur - 1] + diff;
        seq_t next_dec = decimal_of_binary(next_bin);

        /* Resize arrays if necessary */
        if ((size_t)cur >= vals_cap) {
            size_t new_cap = vals_cap ? vals_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(bin_vals, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            bin_vals = tmp;
            tmp = (seq_t *)realloc(dec_vals, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            dec_vals = tmp;
            vals_cap = new_cap;
        }
        bin_vals[cur] = next_bin;
        dec_vals[cur] = next_dec;
        vals_len++;

        /* Append binary digits of the new term to the bits buffer */
        /* Determine bit length of next_bin */
        idx_t bits_of_next = 0;
        seq_t tmp = next_bin;
        while (tmp != (seq_t)0) {
            tmp >>= 1;
            bits_of_next++;
        }
        /* Handle zero explicitly (though it never occurs after init) */
        if (bits_of_next == 0)
            bits_of_next = 1;

        /* Ensure bits buffer capacity */
        if (bits_len + bits_of_next > bits_cap) {
            size_t new_cap = bits_cap ? bits_cap * 2 : 16;
            while (new_cap < bits_len + bits_of_next)
                new_cap *= 2;
            char *tmpb = (char *)realloc(bits, new_cap);
            if (!tmpb) abort();
            bits = tmpb;
            bits_cap = new_cap;
        }

        /* Append bits from MSB to LSB */
        for (idx_t pos = bits_of_next; pos > 0; --pos) {
            bits[bits_len++] = ((next_bin >> (pos - 1)) & 1) ? '1' : '0';
        }

        /* Advance to next bit for the following difference */
        ++bits_pos;
    }

    return dec_vals[n - 1];
}
