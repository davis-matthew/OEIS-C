/*
 * A115823  Integers i such that 8*i XOR 13*i = 21*i.
 * https://oeis.org/A115823
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = i where (8*i) ⊕ (13*i) == 21*i; sequence of such i in
 *            ascending order
 * Range:     u8 n <= 5, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t found = 0;    /* how many solutions we have seen so far */
    seq_t i = 0;        /* candidate integer */

    for (;; i++) {
        seq_t prod8  = i << 3;                  /* 8 * i  */
        seq_t prod13 = i * (seq_t)13;           /* 13 * i */
        seq_t prod21 = i * (seq_t)21;           /* 21 * i */

        if ((prod8 ^ prod13) == prod21) {       /* check the condition */
            if (found == n) return i;           /* the n-th term found */
            found++;                            /* otherwise keep counting */
        }
    }
}
