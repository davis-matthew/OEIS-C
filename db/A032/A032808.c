/*
 * A032808  Numbers whose set of base-8 digits is {2,3}.
 * https://oeis.org/A032808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 62, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base-8 digits are {2,3} in increasing order
   a(n) = the nth integer whose base‑8 representation contains
   only digits 2 and 3.  The sequence is 2, 3, 18, 19, 26, 27, 146, … */

seq_t a(idx_t n)
{
    /* Determine the length (number of base‑8 digits) of the n‑th number.
       Length L satisfies 2^L - 2 < n ≤ 2^(L+1) - 2,
       which is equivalent to L = floor(log2(n+1)). */
    idx_t m = n + (idx_t)1;
    idx_t L = 0;
    while (m > 1) {
        m >>= 1;
        L++;
    }

    /* Number of terms with fewer than L digits: 2^L - 2. */
    idx_t prev_cnt = ((idx_t)1 << L) - (idx_t)2;

    /* 0‑based index of the desired number within the block of length L. */
    idx_t offset = n - prev_cnt - (idx_t)1;

    /* Convert offset into an L‑digit base‑8 number with digits 2 and 3. */
    seq_t value = 0;
    for (idx_t pos = L; pos--; ) {
        seq_t digit = (offset >> pos & (idx_t)1) ? (seq_t)3 : (seq_t)2;
        value = value * (seq_t)8 + digit;
    }
    return value;
}
