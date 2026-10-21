/*
 * A054475  Numbers not divisible by any of their digits when written in base
 *          4.
 * https://oeis.org/A054475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * This function generates the OEIS sequence A054475:
 * Numbers not divisible by any of their base‑4 digits.
 * It uses a simple incremental sieve: starting from 1 it tests
 * each integer, and keeps those that satisfy the property until
 * the desired index `n` is reached.  The sequence is stored
 * in a static dynamic array so that calls with arbitrary `n`
 * are answered in O(n) time and O(n) memory.
 */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static storage for computed terms */
    static seq_t *terms = NULL;   /* dynamic array of terms */
    static size_t terms_len = 0;  /* number of terms stored */
    static size_t terms_cap = 0;  /* capacity of the array */
    static seq_t last_checked = 0;/* last integer tested */

    /* Expand the dynamic array when needed */
    if (terms_len + 1 > terms_cap) {
        size_t new_cap = terms_cap == 0 ? 16 : terms_cap * 2;
        terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        terms_cap = new_cap;
    }

    /* Generate terms until we have at least n of them */
    for (seq_t cur = last_checked + 1;
         terms_len < (size_t)n;
         ++cur) {

        /* test `cur` for the property */
        seq_t temp = cur;
        bool ok = true;
        while (temp > 0) {
            seq_t digit = temp % 4;   /* base‑4 digit */
            temp /= 4;
            if (digit != 0) {        /* ignore zero digits */
                if (cur % digit == 0) {
                    ok = false;
                    break;
                }
            }
        }

        if (ok) {
            /* store the term */
            terms[terms_len++] = cur;
        }
        last_checked = cur;
    }

    /* return the required term (1‑based index) */
    return terms[n - 1];
}
