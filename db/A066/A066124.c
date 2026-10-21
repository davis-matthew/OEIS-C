/*
 * A066124  Numbers that in base 2 need three 'Reverse and Add' steps to reach
 *          a palindrome.
 * https://oeis.org/A066124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 550, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the n‑th integer that needs exactly three Reverse‑and‑Add
 * steps in base 2 to become a binary palindrome. The function generates the
 * sequence by brute force with memoisation.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Reverse the binary representation of x (without leading zeros). */
static inline seq_t rev_bits(seq_t x)
{
    seq_t rev = 0;
    while (x) {
        rev = (rev << 1) | (x & 1);
        x >>= 1;
    }
    return rev;
}

/* Determine whether x requires exactly three Reverse‑and‑Add steps. */
static inline bool is_three_steps(seq_t x)
{
    /* Exclude numbers that are already palindromic. */
    if (x == rev_bits(x))
        return false;

    seq_t cur = x;
    for (int step = 0; step < 3; ++step) {
        seq_t r = rev_bits(cur);
        if (cur == r)               /* became a palindrome before finishing 3 steps */
            return false;
        cur += r;                    /* perform Reverse‑and‑Add */
    }
    return (cur == rev_bits(cur));   /* accept only when palindrome after 3 steps */
}

/* Public sequence entry point */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                   /* behaviour below the offset is insignificant */

    static seq_t *terms = NULL;      /* cached terms */
    static idx_t  size  = 0;         /* number of stored terms   */
    static idx_t  cap   = 0;         /* current allocation size  */

    /* Generate terms until we have at least n of them. */
    while (size < n) {
        seq_t cand = (size == 0) ? (seq_t)1 : terms[size - 1] + 1;
        for (; size < n; ++cand) {
            if (is_three_steps(cand)) {
                if (size == cap) {
                    cap = cap ? cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(terms, cap * sizeof(seq_t));
                    terms = tmp;
                }
                terms[size++] = cand;
            }
        }
    }

    return terms[n - 1];
}
