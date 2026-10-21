/*
 * A061873  Numbers k such that |first digit - second digit + third digit -
 *          fourth digit ...| = 4.
 * https://oeis.org/A061873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   numbers k such that |first digit - second digit + third digit -
 *            fourth digit ...| = 4
 * Range:     u8 n <= 29, u16 n <= 6388, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of already found terms */
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static seq_t next_candidate = 1;  /* next number to test */

    while (terms_len < (size_t)n) {
        /* test next_candidate for the alternating digit sum condition */
        seq_t x = next_candidate;
        /* collect digits in little‑endian order */
        int digits[50];   /* enough for 128‑bit numbers */
        int dcnt = 0;
        while (x > 0) {
            digits[dcnt++] = (int)(x % 10);
            x = x / 10;
        }

        /* compute alternating sum starting with positive sign on the most significant digit */
        int sum = 0;
        int sign = 1;          /* + for first (most significant) digit */
        for (int i = dcnt - 1; i >= 0; --i) {
            sum += sign * digits[i];
            sign = -sign;
        }

        /* check the required absolute value */
        if (sum < 0) sum = -sum;
        if (sum == 4) {
            /* store the term */
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap == 0 ? 16 : terms_cap * 2;
                seq_t *new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!new_terms) return 0;     /* out of memory – return 0 as a safe value */
                terms = new_terms;
                terms_cap = new_cap;
            }
            terms[terms_len++] = next_candidate;
        }

        /* advance to the next number to test */
        next_candidate += 1;
    }

    return terms[n - 1];
}
