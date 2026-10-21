/*
 * A051732  Number of rounds of shuffling required to restore a deck of n
 *          cards to its original order: shuffling is done by keeping first
 *          card, putting second at end of deck, keeping next, putting next at
 *          end and so on.
 * https://oeis.org/A051732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 67, u32 n <= 279, u64 n <= 1003,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Compute a(n) – the order of the keep‑first / move‑second‑to‑bottom
 * shuffle permutation.  For a deck of n cards the permutation σ is
 * obtained by repeatedly taking the top card, writing it out, taking
 * the next card and placing it at the bottom, and so on until all
 * cards are written.  a(n) is the least common multiple of the cycle
 * lengths of σ.  The algorithm runs in O(n) time and uses O(n)
 * auxiliary memory.
 */

/* Standard C headers for memory allocation */
#include <stdlib.h>
#include <stdint.h>

/* Utility: greatest‑common divisor for arbitrary precision seq_t. */
static seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For n = 0 return 0. */
    if (n == (idx_t)0) return (seq_t)0;

    /* Allocate working arrays. */
    size_t len = (size_t)n;                     /* safe cast for malloc */
    idx_t *deck   = (idx_t *)malloc(len * sizeof(idx_t));
    idx_t *perm   = (idx_t *)malloc((len + (size_t)1) * sizeof(idx_t));
    uint8_t *vis  = (uint8_t *)malloc((len + (size_t)1) * sizeof(uint8_t));
    idx_t  *out   = (idx_t *)malloc(len * sizeof(idx_t));

    if (!deck || !perm || !vis || !out) {
        /* Allocation failure – cleanup and return 0. */
        free(deck); free(perm); free(vis); free(out);
        return (seq_t)0;
    }

    /* --- Build one shuffle permutation ---------------------------------- */
    /* Fill the "deck" with card positions 1..n. */
    for (idx_t i = (idx_t)0; i < n; ++i) deck[i] = i + (idx_t)1;

    /* Circular‑buffer indices.  front points to next card to pop.  */
    idx_t front = (idx_t)0;
    idx_t back  = n;           /* next free position at the end */
    idx_t remaining = n;       /* cards still in the deck */
    idx_t out_idx   = (idx_t)0;/* position in the output array */

    while (remaining > (idx_t)0) {
        /* Output the front card. */
        idx_t card = deck[front % n];
        front++;
        out[out_idx++] = card;
        remaining--;

        if (remaining > (idx_t)0) {
            /* Take next card and put it at the back. */
            idx_t mv = deck[front % n];
            front++;
            deck[back % n] = mv;
            back++;
        }
    }

    /* perm[i] = new position of card i.  i ranges from 1..n. */
    for (idx_t pos = (idx_t)1; pos <= n; ++pos) {
        idx_t card = out[pos - (idx_t)1];
        perm[card] = pos;
    }

    /* --- Compute the least common multiple of cycle lengths ---------- */
    for (idx_t i = (idx_t)1; i <= n; ++i) vis[i] = (uint8_t)0;

    seq_t lcm = (seq_t)1;

    for (idx_t i = (idx_t)1; i <= n; ++i) {
        if (vis[i]) continue;
        /* Follow the cycle starting at i. */
        idx_t cur = i;
        idx_t cycle_len = (idx_t)0;
        do {
            vis[cur] = (uint8_t)1;
            cur = perm[cur];
            cycle_len++;
        } while (cur != i);

        seq_t len_seq = (seq_t)cycle_len;
        seq_t g = gcd_seq_t(lcm, len_seq);
        lcm = (lcm / g) * len_seq;
    }

    /* Clean up and return. */
    free(deck); free(perm); free(vis); free(out);
    return lcm;
}
