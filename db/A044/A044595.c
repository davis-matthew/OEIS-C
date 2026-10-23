/*
 * A044595  Numbers n such that string 3,3 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   has33_oct
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes OEIS A044595:
   a(n) = nth positive integer N such that the octal representation of N
   contains the substring "33" but the octal representation of N+1 does not.
   The implementation scans successive integers, checks the condition,
   and caches the result.  It works for any sequence value type seq_t and
   works for arbitrary build sizes.
*/

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has33_oct.h"

/* Cached sequence */
static seq_t *seq = NULL;          /* array of found terms          */
static size_t seq_len = 0;         /* number of stored terms        */
static size_t seq_cap = 0;         /* capacity of the array         */
static seq_t current = 0;          /* last integer examined         */

/* Ensure array has space for at least `need` elements */
static void grow_seq(size_t need)
{
    if (need <= seq_cap)
        return;
    size_t new_cap = seq_cap == 0 ? 8 : seq_cap * 2;
    while (new_cap < need)
        new_cap *= 2;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Generate terms until we have at least `n` terms */
static void generate_until(idx_t n)
{
    while ((size_t)n > seq_len) {
        /* Find next integer satisfying the condition */
        while (true) {
            current += 1;
            if (!has33_oct(current))
                continue;
            /* Check the next integer does NOT have '33' */
            if (!has33_oct(current + 1)) {
                /* Store it */
                grow_seq(seq_len + 1);
                seq[seq_len++] = current;
                break;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) /* Undefined for indices < 1, but caller won't ask for 0 */
        return 0;
    generate_until(n);
    return seq[n - 1];
}
