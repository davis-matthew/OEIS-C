/*
 * A046486  Numbers k such that the sum of the first k palindromes is a
 *          palindrome.
 * https://oeis.org/A046486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, base
 * Formula:   a(n) is the nth integer k such that the sum of the first k
 *            palindromes is a palindrome. The function generates palindromes
 *            in order, keeps a running sum, and records the index whenever
 *            that sum is itself a palindrome
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_palindrome.h"

seq_t a(idx_t n) {
    /* Cache of previously found terms.  The cache grows only when
       needed and is shared between calls to handle non‑sequential
       requests. */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;

    /* State of the on‑the‑fly generator.  All values are kept in the
       same unsigned integer type as the returned data to avoid
       conversions. */
    static seq_t cand   = 1;     /* current integer being tested */
    static seq_t sum    = 0;     /* cumulative sum of palindromes */
    static idx_t  count = 0;     /* number of palindromes processed so far */

    /* The OEIS offset starts at 1; behaviour for n == 0 is
       well‑defined but arbitrary. */
    if (n == 0)
        return 0;

    /* Produce terms until we have at least n cached values. */
    while (len < n) {
        if (is_palindrome(cand, (idx_t)10)) {
            sum += cand;
            ++count;
            if (is_palindrome(sum, (idx_t)10)) {
                /* Ensure cache capacity. */
                if (len == cap) {
                    cap = (cap == 0) ? 4 : cap * 2;
                    cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                }
                /* Store the current index (count) as a cache entry. */
                cache[len++] = (seq_t)count;
            }
        }
        /* Advance to the next integer.  Using seq_t for the loop
           variable keeps overflow behavior consistent with the
           arithmetic restrictions. */
        cand++;
    }

    return cache[n - 1];
}
