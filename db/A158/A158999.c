/*
 * A158999  Palprimes of the form m*reversal(m)+1, sorted on m.
 * https://oeis.org/A158999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, more, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   is_palindrome, is_prime_big, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A158999: palprimes of the form m*rev(m)+1,
   sorted on increasing m (duplicates preserved). */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/rev.h"
#include "../utils/is_palindrome.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                       /* undefined for offset < 1 */
    static idx_t next_m = 1;                  /* next m to test */
    static seq_t *terms = NULL;              /* stored palprime values */
    static size_t term_len = 0, term_cap = 0;

    size_t need = (size_t)n;                  /* target count */

    while (term_len < need) {
        idx_t rev_m = rev(next_m);
        seq_t m   = (seq_t)next_m;
        seq_t rev = (seq_t)rev_m;
        seq_t cand = m * rev + 1;

        if (is_palindrome(cand, 10) && is_prime_big(cand)) {
            if (term_len == term_cap) {
                size_t new_cap = term_cap ? term_cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) {
                    free(terms);
                    exit(1);
                }
                terms = tmp;
                term_cap = new_cap;
            }
            terms[term_len++] = cand;
        }
        ++next_m;
    }
    return terms[need - 1];
}
