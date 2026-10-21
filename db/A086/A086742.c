/*
 * A086742  Start with a(0)=1, then k-th run is 1,2,3,..., a(0) + a(1) + a(2)
 *          + ... + a(k-1).
 * https://oeis.org/A086742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   sequence A086742 – runs of consecutive integers where the k‑th
 *            run is 1,2,…,S_{k-1} with S_{k-1}=sum of all previous terms
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;           /* first term */
    idx_t idx = 1;                  /* index of the next term to be generated */
    seq_t cs   = 1;                 /* cumulative sum of all terms processed so far */

    for (;;) {
        seq_t runlen = cs;          /* length of the next run */
        for (seq_t val = 1; val <= runlen; ++val) {
            if (idx == n) return val;
            ++idx;
            cs += val;
        }
    }
}
