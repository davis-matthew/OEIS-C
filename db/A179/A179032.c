/*
 * A179032  Emirps whose only prime digits are one or more 2's.
 * https://oeis.org/A179032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   Emirps whose only prime digits are one or more 2's
 * Range:     u8 none, u16 n <= 10, u32 n <= 10, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   grow_seq_array, is_prime_big, passes_allowed_and_has2,
 *            reverse_digits, seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* An emirp is a non‑palindromic prime whose reversal is also prime.
   This function generates all such emirps that consist only of the
   decimal digits 0,1,2,4,6,8,9 (i.e. no prime digit other than 2).
   The terms are produced in ascending numeric order.  The routine
   memoises results so that subsequent calls in any order are O(1). */

#include <stdbool.h>

#include "../utils/reverse_digits.h"
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq_array.h"
#include "../utils/seq_alloc_array.h"
#include "../utils/passes_allowed_and_has2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    static seq_t *terms   = NULL; /* array of found emirps */
    static size_t terms_cap = 0;   /* current capacity of terms */
    static idx_t terms_cnt  = 0;   /* number of filled entries */
    static idx_t last_checked = 2; /* last integer examined */

    /* initial allocation */
    if (!terms) {
        terms_cap = 64;
        terms = seq_alloc_array(terms_cap);
        terms_cnt = 0;
    }

    /* generate until we have at least n terms */
    if (n > terms_cnt) {
        bool has2;
        while (terms_cnt < n) {
            /* find next candidate that has only allowed digits
               and contains at least one 2 */
            while (true) {
                if (passes_allowed_and_has2(last_checked, &has2) && has2)
                    break;
                last_checked++;
            }

            seq_t cur = (seq_t)last_checked;

            /* primality tests */
            if (is_prime_big(cur)) {
                seq_t rev = reverse_digits(cur);
                if (rev != cur && is_prime_big(rev)) {
                    /* store the emirp */
                    if (terms_cnt == terms_cap) {
                        grow_seq_array(&terms, &terms_cap, terms_cap * 2);
                    }
                    terms[terms_cnt++] = cur;
                }
            }
            last_checked++; /* move on to next integer */
        }
    }

    return terms[n - 1];
}
