/*
 * A202264  Noncomposite numbers in which all substrings and reversal
 *          substrings are noncomposites.
 * https://oeis.org/A202264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   Noncomposite numbers in which all substrings and reversal
 *            substrings are noncomposite
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

/* Return the n-th term of A202264 (1‑based).  */
seq_t a(idx_t n)
{
    /* Maximum value to search for.  The sequence is known to end
       at 373, so 500 000 is more than sufficient.  */
    static const seq_t SEARCH_LIMIT = (seq_t)500000;
    static seq_t *seq = NULL;      /* array of sequence values     */
    static size_t seq_len = 0;     /* number of stored values     */
    static size_t seq_cap = 0;     /* capacity of the array       */

    /* Helper to ensure capacity of the array.  */
    if (n > seq_len && seq_len == 0) {
        /* First call: generate all terms up to SEARCH_LIMIT.  */
        /* Allocate an initial capacity.  */
        seq_cap = 16;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));

        for (seq_t x = 1; x <= SEARCH_LIMIT; ++x) {
            /* Only consider noncomposite numbers.  */
            if (x != 1 && !isPrimeGeneric(x))
                continue;

            /* Extract decimal digits into an array (most significant first).  */
            uint8_t rev[32];
            size_t rev_len = 0;
            seq_t tmp = x;
            while (tmp > 0) {
                rev[rev_len++] = (uint8_t)(tmp % 10);
                tmp /= 10;
            }
            if (rev_len == 0) rev[rev_len++] = 0;   /* for x==0, never used */
            uint8_t digits[32];
            for (size_t i = 0; i < rev_len; ++i)
                digits[i] = rev[rev_len - 1 - i];

            bool ok = true;
            /* Check all substrings and their reversals.  */
            for (size_t i = 0; i < rev_len && ok; ++i) {
                seq_t sub = 0;
                for (size_t j = i; j < rev_len && ok; ++j) {
                    sub = sub * 10 + digits[j];
                    if (!(sub == 1 || isPrimeGeneric(sub)))
                        ok = false;
                    /* Reversed substring.  */
                    seq_t revsub = 0;
                    for (size_t k = j;; --k) {
                        revsub = revsub * 10 + digits[k];
                        if (!(revsub == 1 || isPrimeGeneric(revsub))) {
                            ok = false;
                            break;
                        }
                        if (k == i) break;
                    }
                }
            }
            if (!ok)
                continue;

            /* Accept the number into the sequence.  */
            if (seq_len >= seq_cap) {
                size_t new_cap = seq_cap * 2;
                seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                seq_cap = new_cap;
            }
            seq[seq_len++] = x;
        }
    }

    if (n == 0 || n > seq_len)
        return (seq_t)0;
    return seq[n - 1];
}
