/*
 * A121541  Increasing sequence: "if n appears a*n+b does not", case a(1)=4,
 *          a=2, b=1.
 * https://oeis.org/A121541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A121541
   a(1) = 4.
   For n > 1, a(n) is the smallest integer greater than a(n‑1) that is
   not of the form 2·k+1 for any k already appearing in the sequence.
   The generation rule can be seen as:
      start with included set {4};
      when we include x, we mark 2·x+1 as excluded.
      The next term is the smallest integer not yet excluded.
*/

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    /* offset 1 – if called with 0 return 0 (unused) */
    if (n == 0) return (seq_t)0;

    /* Persistent state across calls */
    static unsigned char *excluded = NULL;   /* bit mask of excluded numbers   */
    static size_t       excl_cap   = 0;       /* capacity in bytes              */
    static idx_t        next_i     = 4;        /* next integer to test           */

    static seq_t *seq      = NULL;            /* array of sequence terms        */
    static size_t  seq_cap  = 0;              /* capacity of seq array          */
    static size_t  seq_len  = 0;              /* number of terms generated      */

    /* Initialise exclusion array on first call */
    if (excluded == NULL) {
        excl_cap   = 128;
        excluded   = (unsigned char*)calloc(excl_cap, 1);
    }

    /* Initialise sequence array on first call */
    if (seq == NULL) {
        seq_cap = 16;
        seq     = (seq_t*)malloc(seq_cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them */
    while (seq_len < (size_t)n) {
        /* Grow exclusion array if current candidate exceeds capacity */
        if ((size_t)next_i >= excl_cap) {
            size_t new_cap = excl_cap;
            while (new_cap <= (size_t)next_i) new_cap <<= 1;
            unsigned char *tmp = (unsigned char*)realloc(excluded, new_cap);
            memset(tmp + excl_cap, 0, new_cap - excl_cap);
            excluded = tmp;
            excl_cap = new_cap;
        }

        if (!excluded[(size_t)next_i]) { /* include this number */
            /* Grow sequence array if needed */
            if (seq_len >= seq_cap) {
                size_t new_cap = seq_cap ? seq_cap << 1 : 16;
                seq_t *tmp = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
                seq = tmp;
                seq_cap = new_cap;
            }
            seq[seq_len++] = (seq_t)next_i;   /* store it */

            /* Exclude 2*next_i + 1 */
            idx_t j = 2 * next_i + 1;
            if ((size_t)j >= excl_cap) {
                size_t new_cap = excl_cap;
                while (new_cap <= (size_t)j) new_cap <<= 1;
                unsigned char *tmp = (unsigned char*)realloc(excluded, new_cap);
                memset(tmp + excl_cap, 0, new_cap - excl_cap);
                excluded = tmp;
                excl_cap = new_cap;
            }
            excluded[(size_t)j] = 1;
        }
        ++next_i;   /* advance to next candidate */
    }

    return seq[n - 1];
}
