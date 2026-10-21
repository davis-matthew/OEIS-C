/*
 * A099280  2^n-th palindromic number.
 * https://oeis.org/A099280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A099280 : a(n) = the 2^n‑th positive base‑10 palindrome
 *  The algorithm enumerates palindromes by ordinary length,
 *  uses the cumulative count to find the block containing the 2^n‑th
 *  element, then builds the palindrome from its left half.
 *  The implementation works for any seq_t (integer, unsigned __int128
 *  or arbitrary‑precision) and any idx_t (unsigned integer type).
 */

#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Helper: power of 10 – 10^e   (seq_t may be big integer).
 * Performs e  multiplies by 10;  e fits in idx_t.
 */
static seq_t pow10_idx(idx_t e)
{
    seq_t r = (seq_t)1;
    while (e--) r *= (seq_t)10;
    return r;
}

/* ------------------------------------------------------------------ */
/* Helper: power of 2 – 2^e   (seq_t may be big integer).
 */
static seq_t pow2(idx_t e)
{
    seq_t r = (seq_t)1;
    while (e--) r *= (seq_t)2;
    return r;
}

/* ------------------------------------------------------------------ */
/* Helper: build a palindrome from left half.
 *  first_half – the integer consisting of the left half of the digits.
 *  odd       – true if the total length is odd (there is a central digit).
 *
 *  The palindrome is constructed by reflecting first_half (or first_half
 *  without its last digit if odd) onto the right side.
 */
static seq_t make_pal(seq_t first_half, bool odd)
{
    seq_t pal = first_half;
    seq_t tmp = first_half;
    if (odd) tmp /= (seq_t)10;        /* drop the middle digit */
    while (tmp > (seq_t)0) {
        pal = pal * (seq_t)10 + (tmp % (seq_t)10);
        tmp /= (seq_t)10;
    }
    return pal;
}

/* ------------------------------------------------------------------ */
/* The required sequence element. */
seq_t a(idx_t n)
{
    /* k = 2^n  (1‑indexed rank of the palindrome) */
    seq_t k = pow2(n);

    seq_t cum = (seq_t)0;   /* cumulative count of palindromes so far */

    for (idx_t L = 1; ; ++L) {
        idx_t half = (L + 1) / 2;                                 /* first half length */
        seq_t count = (seq_t)9 * pow10_idx(half - 1);             /* palindromes of length L */

        if (cum + count >= k) {                                  /* desired palindrome in this block */
            seq_t offset = k - cum - (seq_t)1;                    /* 0‑based offset within this length */
            seq_t first_half = pow10_idx(half - 1) + offset;      /* left half as an integer */

            bool odd = (L & 1u) != 0;                             /* odd length? */
            return make_pal(first_half, odd);                    /* construct full palindrome */
        }
        cum += count;                                            /* move to next length block */
    }
}
