/*
 * A075943  List of codewords in binary lexicode with Hamming distance 9
 *          written as decimal numbers.
 * https://oeis.org/A075943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th element of the greedy binary lexicode with
 *            minimum Hamming distance 9, constructed by taking the smallest
 *            integer that is at Hamming distance at least 9 from every
 *            previously selected codeword
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state persistent between calls */
    static seq_t *code = NULL;          /* array of accepted codewords   */
    static idx_t   code_len = 0;        /* number of codewords stored    */
    static idx_t   code_cap = 0;        /* allocated capacity            */
    static seq_t  next_candidate = 0;   /* next integer to test          */

    /* --- initialization ------------------------------------------- */
    if (code_len == 0) {
        /* start with 0 as first codeword */
        code_cap = 4;
        code = (seq_t *)realloc(NULL, code_cap * sizeof(seq_t));
        code[0] = 0;
        code_len = 1;
        next_candidate = 1;   /* candidate numbers are tested in ascending order */
    }

    /* --- grow sequence until it contains at least n+1 terms -------- */
    while (code_len <= n) {
        seq_t cand = next_candidate;        /* candidate codeword          */
        bool   ok   = true;                 /* true iff distance >= 9 to all */

        /* check distance to all previously accepted codewords */
        idx_t i;
        for (i = 0; i < code_len && ok; ++i) {
            seq_t diff = cand ^ code[i];
            seq_t tmp  = diff;
            seq_t zero = 0;
            size_t bits = 0;
            /* pop‑count of diff: count set bits */
            while (tmp != zero) {
                tmp &= tmp - 1;
                bits++;
            }
            if (bits < 9) ok = false;
        }

        if (ok) { /* accept candidate */
            if (code_len == code_cap) {
                code_cap = code_cap ? code_cap * 2 : 4;
                code = (seq_t *)realloc(code, code_cap * sizeof(seq_t));
            }
            code[code_len++] = cand;
        }
        /* advance to next integer to test */
        next_candidate += 1;
    }

    return code[n];
}
