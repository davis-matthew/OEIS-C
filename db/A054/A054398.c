/*
 * A054398  Define a sequence of 2^n X 2^n squares as follows: S_0 = [1], S_1
 *          = [1,2; 3,4]; S_2 = [1,2,5,6; 3,4,7,8; 9,10,13,14; 11,12,15,16],
 *          etc.; sequence gives triangular array whose n-th row gives
 *          differences between successive columns of n-th square.
 * https://oeis.org/A054398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy, nice
 * Formula:   a(n) = (2^(2*t+1) + 1) / 3 where t is the number of trailing
 *            1-bits in the binary representation of the offset of n within
 *            its row (the row is the minimal r≥1 such that the first r rows
 *            contain at least n terms)
 * Range:     u8 n <= 41, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS compute function for A054398 */
seq_t a(idx_t n)
{
    /* Find the row r for the index n.
       cum_before  = number of terms in all preceding rows */
    idx_t r = 1;
    idx_t cum_before = 0;
    idx_t row_len;

    while (1)
    {
        row_len = (idx_t)(((seq_t)1 << r) - 1);   /* length of row r = 2^r – 1 */
        if (cum_before + row_len >= n)
            break;
        cum_before += row_len;
        ++r;
    }

    /* Position j (0‑based) within the found row */
    idx_t j = n - cum_before - 1;

    /* Count trailing 1‑bits in j. */
    idx_t tmp = j;
    idx_t t = 0;
    while ((tmp & (idx_t)1) == (idx_t)1)
    {
        ++t;
        tmp >>= 1;
    }

    /* Compute the value (2^(2*t+1) + 1) / 3. */
    seq_t pow2 = ((seq_t)1 << (2 * t + 1));
    return (pow2 + (seq_t)1) / (seq_t)3;
}
