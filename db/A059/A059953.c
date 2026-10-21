/*
 * A059953  Ordering of a deck of 52 cards after an out-shuffle.
 * https://oeis.org/A059953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, full, changed
 * Formula:   a(n) = (n is odd) ? (n+1)/2 : 26 + n/2 (modification of a
 *            perfect out‑shuffle)
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This gives the original card position that ends up in position n after a single out‑shuffle
   of a 52‑card deck.  For n outside the deck size (n>52) the function returns 0. */
seq_t a(idx_t n)
{
    /* Avoid out‑of‑range accesses; the permutation is only defined for n = 1..52. */
    if (n == 0 || n > 52)
        return (seq_t)0;

    /* r = floor(n/2) works for both odd and even indices. */
    idx_t r = n >> 1;          /* equivalent to n / 2 */

    if (n & 1)                 /* odd n => first half of the deck */
    {
        return (seq_t)(r + 1U);
    }
    else                        /* even n => second half of the deck */
    {
        return (seq_t)(26U + r);
    }
}
