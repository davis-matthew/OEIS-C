/*
 * A057557  Lexicographic ordering of NxNxN, where N={1,2,3,...}.
 * https://oeis.org/A057557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth element of the flattened list of all
 *            positive‑integer triples (i,j,k) ordered by increasing sum
 *            i+j+k, and for equal sums ordered lexicographically by (i,j).
 *            The sequence is obtained by writing each triple as i, j, k and
 *            concatenating the numbers
 * Range:     u8 n <= 168, u16 n <= 168, u32 n <= 168, u64 n <= 168,
 *            u128 n <= 168, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Return the nth term (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of range – undefined behaviour */

    /* The nth term belongs to triple number t = ceil(n/3). */
    idx_t t = (n + 2) / 3;          /* integer division */

    /* Find the sum S of coordinates for this triple:
       the first S for which C(S,3) ≥ t.  C(S,3)=S*(S-1)*(S-2)/6. */
    idx_t S = 3;
    for (;;) {
        seq_t cnt = (seq_t)S * (S - 1) * (S - 2) / 6;
        if (cnt >= t) break;
        ++S;
    }

    /* Number of triples before this sum layer. */
    seq_t prev = (S > 3) ? ((seq_t)(S - 1) * (S - 2) * (S - 3) / 6) : 0;
    idx_t pos = (idx_t)(t - prev); /* 1‑based position inside the S‑layer */

    /* Find the first coordinate i.  In the S‑layer there are
       blocks of size (S-i-1) corresponding to fixed i and
       varying j from 1 to S-i-1. */
    idx_t i = 1;
    for (;;) {
        idx_t block = S - i - 1;
        if (pos <= block) break;
        pos -= block;
        ++i;
    }

    /* Now pos is the position of j within the current block. */
    idx_t j = pos;
    idx_t k = S - i - j;           /* k is fixed by the sum constraint */

    /* Return the coordinate that corresponds to the original n. */
    switch (n % 3) {               /* 1 → i, 2 → j, 0 → k */
        case 1: return (seq_t)i;
        case 2: return (seq_t)j;
        default: return (seq_t)k;
    }
}
