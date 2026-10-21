/*
 * A067936  Let rep(n) be the n-th repunit number, sequence gives values of n
 *          such that : rep(n) == rep(3) (mod n).
 * https://oeis.org/A067936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   rep_mod_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: rep(n) = (10^n - 1)/9 (repunit).  
   Condition: rep(n) ≡ rep(3) (mod n).  Since rep(3)=111,  
   we test  rep(n) mod n == 111 mod n.  The function
   builds the list of all such n lazily, storing them in a static
   cache so that any n can be requested in arbitrary order. */

#include <stdlib.h>
#include "../utils/rep_mod_n.h"

/* static cache of terms: terms[i] == a(i+1) for i>=0 */
static idx_t *terms = NULL;   /* array of n's that satisfy the condition */
static size_t terms_len = 0;   /* number of terms cached */
static size_t terms_cap = 0;   /* allocated capacity of the array */
static idx_t next_candidate = 1;  /* candidate m to test next */

seq_t a(idx_t n)
{
    /* n is 1‑based.  Ensure cache contains at least n terms. */
    while (terms_len < n) {
        /* Search for the next integer m that satisfies the condition. */
        while (1) {
            idx_t m = next_candidate++;
            if (m == 0) continue;         /* protection against overflow */

            idx_t rep_mod = rep_mod_n(m);
            idx_t rhs = 111 % m;          /* rep(3) mod m */

            if (rep_mod == rhs) {
                /* Grow the cache if necessary. */
                if (terms_len == terms_cap) {
                    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                    idx_t *tmp = (idx_t *)realloc(terms, new_cap * sizeof(idx_t));
                    if (!tmp) exit(1);        /* allocation failure */
                    terms = tmp;
                    terms_cap = new_cap;
                }
                terms[terms_len++] = m;
                break; /* found next term, move to outer while */
            }
        }
    }

    /* Return the requested term converted to seq_t. */
    return (seq_t)terms[n - 1];
}
