/*
 * A075934  List of codewords in binary lexicode with Hamming distance 6
 *          written as decimal numbers.
 * https://oeis.org/A075934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   distance_le5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function constructs the lexicographically smallest set of binary 
 * codewords with minimum Hamming distance 6.
 * It returns the n‑th codeword as a decimal integer (a(0) = 0). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/distance_le5.h"

seq_t a(idx_t n)
{
    /* static state needed for memoisation */
    static seq_t *codes = NULL;
    static size_t codes_size = 0;    /* number of codewords stored */
    static size_t codes_cap  = 0;    /* allocated capacity */
    static seq_t next_candidate = (seq_t)1; /* next integer to test */

    /* initialise on first call */
    if (!codes) {
        /* initial capacity */
        codes_cap = 256;
        codes = (seq_t *)malloc(codes_cap * sizeof(seq_t));
        if (!codes) return (seq_t)0; /* out of memory */
        /* first codeword is 0 */
        codes[0] = (seq_t)0;
        codes_size = 1;
    }

    /* generate codewords until we have at least (n+1) entries */
    while (codes_size <= n) {
        seq_t cand = next_candidate;
        bool ok = true;
        /* check distance to all existing codewords */
        for (size_t i = 0; i < codes_size; ++i) {
            if (distance_le5(cand, codes[i])) { /* distance <= 5 -> reject */
                ok = false;
                break;
            }
        }
        if (ok) {
            /* append the new codeword, growing the array if necessary */
            if (codes_size == codes_cap) {
                codes_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(codes, codes_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0; /* out of memory */
                codes = tmp;
            }
            codes[codes_size++] = cand;
        }
        next_candidate = cand + (seq_t)1;
    }

    return codes[n];
}
