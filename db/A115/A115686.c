/*
 * A115686  Powerful(1) numbers (A001694) whose digit reversal is a prime
 *          number.
 * https://oeis.org/A115686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   append_candidate, compare_seq_t, is_prime_big, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A115686: Powerful(1) numbers whose decimal reverse is prime
 *
 * The sequence is computed by generating all powerful numbers N
 * of the form a^2 * b^3 up to a sufficiently large limit (1e10),
 * keeping those whose decimal reversal rev(N) is prime.
 * The resulting numbers are sorted, deduplicated, and stored
 * in a static array.  The function returns the (n‑1)‑indexed
 * element of that array.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/append_candidate.h"
#include "../utils/compare_seq_t.h"
#include "../utils/is_prime_big.h"
#include "../utils/rev.h"

seq_t a(idx_t n)
{
    /* cache of the generated sequence */
    static seq_t *seq   = NULL;
    static size_t  seq_len = 0;

    if (seq_len == 0) {
        /* ----------------------------------------------------------
         *  Build the whole sequence once.
         * ---------------------------------------------------------- */
        const idx_t LIMIT = 10000000000ULL;  /* 1·10^10, more than enough for 10 000 terms */

        seq_t *tmp       = NULL;
        size_t len       = 0;
        size_t cap       = 0;

        /* enumerate a²*b³ <= LIMIT */
        for (idx_t a = 1; (idx_t)a * a <= LIMIT; ++a) {
            seq_t a2 = (seq_t)a * (seq_t)a;          /* a²  */
            for (idx_t b = 1;; ++b) {
                seq_t b3 = (seq_t)b * (seq_t)b * (seq_t)b;  /* b³  */
                seq_t val = a2 * b3;
                if (val > (seq_t)LIMIT)
                    break;

                /* reverse in decimal and test primality */
                if (is_prime_big((seq_t)rev((idx_t)val)))
                    append_candidate(&tmp, &len, &cap, val);
            }
        }

        /* sort the candidates */
        if (len > 1)
            qsort(tmp, len, sizeof(seq_t), compare_seq_t);

        /* deduplicate */
        size_t uniq = 0;
        for (size_t i = 0; i < len; ++i) {
            if (uniq == 0 || compare_seq_t(&tmp[uniq - 1], &tmp[i]) != 0)
                tmp[uniq++] = tmp[i];
        }

        seq     = tmp;
        seq_len = uniq;
    }

    /* the sequence is 1‑based in OEIS */
    return seq[n - 1];
}
