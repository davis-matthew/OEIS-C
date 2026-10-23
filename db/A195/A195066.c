/*
 * A195066  Numbers n such that BCR(n) is not less than n, where BCR =
 *          binary-complement-and-reverse = A036044.
 * https://oeis.org/A195066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A195066: numbers n such that BCR(n) ≥ n,
   where BCR(n) = binary‑complement‑and‑reverse of n. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <stdbool.h>

/* Compute the number of bits of a seq_t value, treating 0 as
   having 1 bit.  Works for any unsigned integer type or
   arbitrary‑precision integer class that supports >> and >. */
static inline unsigned bitlen_seq_t(seq_t x)
{
    if (x == 0) return 1;
    unsigned len = 0;
    seq_t y = x;
    while (y > 0) {
        ++len;
        y >>= 1;
    }
    return len;
}

/* BCR(n): complement of n within its minimal bit length, then reverse. */
static inline seq_t bcr(seq_t n)
{
    unsigned bits = bitlen_seq_t(n);                      /* at least 1 */
    size_t type_bits = sizeof(seq_t) * CHAR_BIT;          /* size of the underlying type */

    seq_t mask;
    if (bits >= type_bits)
        mask = (seq_t) -1;                               /* all bits 1 */
    else
        mask = ((seq_t)1 << bits) - 1;                    /* lower 'bits' bits 1 */

    seq_t rev = (~n) & mask;                             /* bitwise complement within bits */
    seq_t res = 0;
    for (unsigned i = 0; i < bits; ++i) {
        res = (res << 1) | ((rev >> i) & (seq_t)1);
    }
    return res;
}

/* Memoisation structures */
static idx_t   next_test = 0;          /* next integer to test */
static size_t  terms_cnt = 0;          /* how many terms stored */
static seq_t  *terms     = NULL;       /* array of found terms */
static size_t  terms_cap = 0;          /* current capacity */

/* Ensure capacity for at least 'needed' terms. */
static void grow_terms(size_t needed)
{
    if (needed <= terms_cap) return;
    size_t newcap = terms_cap ? terms_cap * 2 : 16;
    while (newcap < needed) newcap *= 2;
    seq_t *tmp = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
    if (!tmp && newcap > 0) exit(EXIT_FAILURE);
    terms = tmp;
    terms_cap = newcap;
}

/* Return the n-th term of the sequence (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* safety, although offset starts at 1 */

    while (terms_cnt < (size_t)n) {
        seq_t bn = bcr((seq_t)next_test);
        if (bn >= (seq_t)next_test) {
            grow_terms(terms_cnt + 1);
            terms[terms_cnt++] = (seq_t)next_test;
        }
        ++next_test;
    }
    return terms[n - 1];
}
