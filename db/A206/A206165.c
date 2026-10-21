/*
 * A206165  Number of (n+1)X5 0..3 arrays with every 2X2 subblock in a row
 *          having an equal number of equal diagonal or equal antidiagonal
 *          elements, adjacent rows differing in this number, and new values
 *          0..3 introduced in row major order.
 * https://oeis.org/A206165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) +2697*a(n-2) -2261*a(n-3) -62984*a(n-4)
 *            +18588*a(n-5) +148188*a(n-6) +441756*a(n-7) -1398384*a(n-8)
 *            +632448*a(n-9) +6137856*a(n-10) for n>11; initial values:
 *            a(1)=5257, ... , a(11)=716565655722012651878
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_from_str.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

static const char *init_strs[11] = {
    "5257",
    "196078",
    "13899222",
    "526154358",
    "37242588902",
    "1412613762182",
    "99795262195942",
    "3792559398258150",
    "267412693827257062",
    "10182112342596159814",
    "716565655722012651878"
};

static seq_t init_terms[11];
static int init_done = 0;

static void init_initial_terms(void) {
    if (init_done) return;
    for (int i = 0; i < 11; ++i) {
        init_terms[i] = seq_from_str(init_strs[i]);
    }
    init_done = 1;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* outside the domain of the sequence */
    init_initial_terms();

    if (n <= 11) {
        return init_terms[n - 1];
    }

    /* Allocate space for the sequence up to n */
    seq_t *seq = seq_alloc_new_array((size_t)n);
    if (!seq) return (seq_t)0;  /* allocation failure – return zero */

    /* Copy the first 11 terms */
    for (int i = 0; i < 11; ++i) {
        seq[i] = init_terms[i];
    }

    /* Recurrence coefficients */
    const int pos_coefs[7]   = { 1, 2697, 18588, 148188, 441756, 632448, 6137856 };
    const int pos_offs[7]    = { 1, 2, 5, 6, 7, 9, 10 };
    const int neg_coefs[3]   = { 2261, 62984, 1398384 };
    const int neg_offs[3]    = { 3, 4, 8 };

    for (idx_t i = 12; i <= n; ++i) {
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;
        size_t idx = (size_t)(i - 1);      /* current index in the array */

        for (int k = 0; k < 7; ++k) {
            size_t j = (size_t)(i - pos_offs[k] - 1);
            sum_pos += seq[j] * (seq_t)pos_coefs[k];
        }
        for (int k = 0; k < 3; ++k) {
            size_t j = (size_t)(i - neg_offs[k] - 1);
            sum_neg += seq[j] * (seq_t)neg_coefs[k];
        }
        seq[idx] = sum_pos - sum_neg;
    }

    seq_t result = seq[(size_t)(n - 1)];
    seq_free_array(seq);
    return result;
}
