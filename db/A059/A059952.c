/*
 * A059952  Ordering of a deck of 52 cards after an in-shuffle.
 * https://oeis.org/A059952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, full, changed
 * Formula:   a(n) = 27 * n (mod 53) (with 53 interpreted as 0 → 53)
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This gives the ordering of a 52‑card deck after a perfect in‑shuffle. */

seq_t a(idx_t n)
{
    /* 53 is the deck size plus one, the modulo used by a shuffling
       permutation of a 52‑card deck.  The modular inverse of 2 modulo 53
       is 27, so for an in‑shuffle the mapping from position i to new
       position j is j = 2*i (mod 53).  The sequence listed in OEIS is the
       inverse mapping: a(j) = the card that ends up at position j after
       the shuffle, i.e. a(j) = 27*j (mod 53). */
    idx_t m = n % 53;                 /* reduce n modulo 53 first */
    idx_t v = (27 * m) % 53;          /* 27 is the inverse of 2 mod 53  */
    if (v == 0) v = 53;              /* map 0 to the last position 53 (i.e. 52) */
    return (seq_t)v;
}
