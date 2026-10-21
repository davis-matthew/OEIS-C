/*
 * A065191  Limit of the recursion B_[k] = T[k](B_[k-1]), where B_[1] =
 *          (1,2,3,4,5,...) and T[k] is the transformation that permutes the
 *          entries k(2i-1) + j and k(2i) + j for all j = 0,..,k-1 and
 *          positive integers i.
 * https://oeis.org/A065191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A065191: limit of the block‑swap recursion.
   The value at position n is found by retracing the
   effect of the inverses of the block‑swap transformations
   from k = n down to 2.  For each k a position ≥ k is
   shifted down by k if it lies in the second block of its
   2k‑pair, otherwise it is shifted up by k.  The algorithm
   runs in O(n) time and uses only integer arithmetic. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* value below the offset is irrelevant */

    seq_t pos = (seq_t)n;            /* current position of the element */
    for (idx_t k = n; k > 1; --k) {
        seq_t sk   = (seq_t)k;          /* k as seq_t for arithmetic */
        if (pos < sk)
            continue;                   /* positions < k are unchanged */

        seq_t off   = pos - sk;          /* offset from the start of block 1 */
        seq_t block = (seq_t)(2 * k);    /* length of the 2k pair */
        seq_t r = off % block;           /* position inside the pair */

        if (r < sk)          /* element in the first block of the pair */
            pos += sk;       /* move up by k */
        else                  /* element in the second block */
            pos -= sk;       /* move down by k */
    }
    return pos;   /* original index that ends up at position n */
}
