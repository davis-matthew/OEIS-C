/*
 * A085459  Numbers k such that k-th cyclotomic polynomial has exactly 3
 *          positive coefficients.
 * https://oeis.org/A085459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A085459: numbers k such that the k‑th cyclotomic polynomial
   has exactly three positive coefficients.
   The set of such k is the sorted union of
        {3^i | i > 0}
   and
        {2^i * 5^j | i > 0, j > 0}.
   The function below generates terms on demand by merging the two
   infinite sequences.
*/

#include <stdlib.h>
#include <stdbool.h>

/* Helper structure for generating 2^i * 5^j numbers with i,j>0. */
static struct HammingGen {
    seq_t *values;          /* stored 2^a * 5^b values */
    size_t len, cap;        /* current length and capacity */
    size_t i2, i5;          /* indices for next *2 and *5 */
    size_t valid_idx;       /* next index with both exponents > 0 */
} *hamming_gen = NULL;
static bool hamming_inited = false;

static seq_t get_next_hamming2_5(void)
{
    if (!hamming_inited) {
        hamming_gen = (struct HammingGen *)malloc(sizeof(*hamming_gen));
        hamming_gen->cap  = 128;
        hamming_gen->values = (seq_t *)malloc(hamming_gen->cap * sizeof(seq_t));
        hamming_gen->len   = 1;
        hamming_gen->values[0] = (seq_t)1;   /* start with 1 */
        hamming_gen->i2 = hamming_gen->i5 = 0;
        hamming_gen->valid_idx = 0;
        hamming_inited = true;
    }

    while (true) {
        /* generate next value when needed */
        if (hamming_gen->valid_idx >= hamming_gen->len) {
            seq_t v2 = hamming_gen->values[hamming_gen->i2] * (seq_t)2;
            seq_t v5 = hamming_gen->values[hamming_gen->i5] * (seq_t)5;
            seq_t nxt = v2 < v5 ? v2 : v5;
            if (nxt == v2) hamming_gen->i2++;
            if (nxt == v5) hamming_gen->i5++;

            if (hamming_gen->len == hamming_gen->cap) {
                hamming_gen->cap <<= 1;
                hamming_gen->values =
                    (seq_t *)realloc(hamming_gen->values,
                                     hamming_gen->cap * sizeof(seq_t));
            }
            hamming_gen->values[hamming_gen->len++] = nxt;
        }

        seq_t cand = hamming_gen->values[hamming_gen->valid_idx++];
        /* keep only numbers with both exponents > 0 */
        if ((cand % (seq_t)10) == 0) {
            return cand;
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, return 0 for safety */

    /* Cached merged sequence */
    static seq_t *merged = NULL;
    static size_t merged_len = 0, merged_cap = 0;

    /* Current values of the two generators */
    static seq_t pow3_cur = 0;
    static bool pow3_start = false;
    static seq_t hamming_cur = 0;
    static bool hamming_start = false;

    /* Generate until we have at least n terms */
    while (merged_len < n) {
        if (!pow3_start) { pow3_cur = (seq_t)3; pow3_start = true; }
        if (!hamming_start) {
            hamming_cur = get_next_hamming2_5();
            hamming_start = true;
        }

        seq_t next;
        if (pow3_cur < hamming_cur) {
            next = pow3_cur;
            pow3_cur *= (seq_t)3;
        } else {
            next  = hamming_cur;
            hamming_cur = get_next_hamming2_5();
        }

        /* store the next term */
        if (merged_len == merged_cap) {
            merged_cap = merged_cap ? merged_cap << 1 : 128;
            merged = (seq_t *)realloc(merged,
                                      merged_cap * sizeof(seq_t));
        }
        merged[merged_len++] = next;
    }

    return merged[n - 1];
}
