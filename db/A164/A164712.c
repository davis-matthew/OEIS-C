/*
 * A164712  A positive integer n is included if all runs of 0's in binary n
 *          are of the same length, and if there are at least two runs of 0's.
 * https://oeis.org/A164712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: a(n) = nth positive integer whose binary representation
   contains at least two runs of zeros, all of equal length.  The function
   generates the sequence on demand and caches previously found terms so
   that calls in any order are efficient.  */

/* --- includes ---------------------------------------------------------------- */
#include <stdlib.h>
#include <stdbool.h>

/* --- forward declarations ---------------------------------------------------- */
static bool qualifies(seq_t m);

/* --- static storage for caching terms --------------------------------------- */
static seq_t *terms = NULL;    /* array of found terms */
static size_t terms_cap = 0;   /* allocated capacity of `terms` */
static size_t terms_cnt = 0;   /* number of terms stored */
static seq_t last_checked = 0;

/* --- helper to test whether a number satisfies the condition --------------- */
static bool qualifies(seq_t m)
{
    seq_t temp = m;
    int prev_bit = -1;        /* -1 indicates we have not started yet      */
    seq_t run_len = 0;
    seq_t zero_len = 0;
    bool zero_len_set = false;
    int zero_runs = 0;

    while (temp != 0) {
        int bit = (int)(temp & ((seq_t)1));

        if (prev_bit == -1) {
            /* First bit of the representation */
            prev_bit = bit;
            run_len = 1;
        } else if (bit == prev_bit) {
            run_len++;
        } else {
            /* Run just ended */
            if (prev_bit == 0) {
                if (!zero_len_set) {
                    zero_len = run_len;
                    zero_len_set = true;
                } else if (run_len != zero_len) {
                    return false;
                }
                zero_runs++;
            }
            prev_bit = bit;
            run_len = 1;
        }

        /* early exit: a zero run already longer than the common length */
        if (prev_bit == 0 && zero_len_set && run_len > zero_len) {
            return false;
        }

        temp >>= 1;
    }

    /* Process the final run */
    if (prev_bit == 0) {
        if (!zero_len_set) {
            zero_len = run_len;
            zero_len_set = true;
        } else if (run_len != zero_len) {
            return false;
        }
        zero_runs++;
    }

    return zero_runs >= 2;
}

/* --- main sequence function ------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n=0, but harmless */

    /* initialise cache on first call */
    if (terms == NULL) {
        terms_cap = 16;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    }

    /* generate terms until we have at least n of them */
    while (terms_cnt < (size_t)n) {
        last_checked++;
        if (qualifies(last_checked)) {
            if (terms_cnt == terms_cap) {
                terms_cap = (terms_cap == 0 ? 16 : terms_cap * 2);
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_cnt++] = last_checked;
        }
    }

    return terms[(size_t)n - 1];
}
