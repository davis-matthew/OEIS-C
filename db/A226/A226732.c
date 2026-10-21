/*
 * A226732  Palindromes that cannot be expressed as a nontrivial product of
 *          two palindromes.
 * https://oeis.org/A226732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, is_palindrome, pow10, rev_prefix
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A226732: Palindromes that cannot be expressed
   as a nontrivial product of two palindromes (base 10).  */

/* The function follows the definition directly:  a(1)=1, a(2)=2, ...  */
/* It generates all decimal palindromes in increasing order, checks each
   for factorisation into two shorter palindromes, and stores the ones
   that fail that test.  The implementation works for built‑in integer
   types as well as for the arbitrary‑precision class `seq_t`.  */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_palindrome.h"
#include "../utils/floor_sqrt.h"
#include "../utils/pow10.h"
#include "../utils/rev_prefix.h"

/* Generate the next decimal palindrome in increasing order.
   The function uses static state, so consecutive calls produce
   successive palindromes. */
static seq_t next_palindrome(void)
{
    static idx_t len   = 1;            /* current palindrome length            */
    static idx_t half  = 1;            /* ceil(len/2)                          */
    static idx_t start = 1;            /* first prefix for current len          */
    static idx_t end   = 9;            /* last prefix  for current len          */
    static idx_t prefix = 1;           /* current prefix                       */

    seq_t pal;
    if ((len & 1) == 0) {              /* even length */
        pal = (seq_t)prefix * pow10(half) + rev_prefix(prefix);
    } else {                           /* odd  length */
        pal = (seq_t)prefix * pow10(half - 1) +
              rev_prefix(prefix / 10);
    }

    /* Advance generator state for the next call */
    ++prefix;
    if (prefix > end) {
        ++len;
        half = (len + 1) / 2;
        start = (idx_t)pow10(half - 1);
        end   = (idx_t)pow10(half) - 1;
        prefix = start;
    }
    return pal;
}

/* Main routine: return the nth term (1‑based).  */
seq_t a(idx_t n)
{
    /* Storage for generated palindromes and result terms. */
    static seq_t *pal_list = NULL;   /* all palindromes generated so far   */
    static size_t pal_len  = 0, pal_cap = 0;
    static seq_t *res      = NULL;   /* terms of the sequence             */
    static size_t res_len  = 0, res_cap = 0;

    /* Ensure we have computed enough terms. */
    while (res_len < n) {
        seq_t p = next_palindrome();                    /* candidate     */

        /* Check whether p factors into two palindromes. */
        seq_t sqrt_p = floor_sqrt(p);
        bool is_valid = true;
        for (size_t j = 0; j < pal_len; ++j) {
            seq_t d = pal_list[j];
            if (d > sqrt_p) break;                      /* beyond sqrt   */
            if (d <= 1) continue;                       /* trivial factor*/
            if (p % d == 0) {
                seq_t q = p / d;
                if (is_palindrome(q, 10)) {            /* both palindromes? */
                    is_valid = false;
                    break;
                }
            }
        }

        /* Append p to the full palindrome list. */
        if (pal_len == pal_cap) {
            pal_cap = pal_cap ? pal_cap * 2 : 8;
            pal_list = (seq_t*)realloc(pal_list, pal_cap * sizeof(seq_t));
        }
        pal_list[pal_len++] = p;

        /* If p is a valid term, store it. */
        if (is_valid) {
            if (res_len == res_cap) {
                res_cap = res_cap ? res_cap * 2 : 8;
                res = (seq_t*)realloc(res, res_cap * sizeof(seq_t));
            }
            res[res_len++] = p;
        }
    }

    return res[n - 1];   /* sequence is 1‑based */
}
