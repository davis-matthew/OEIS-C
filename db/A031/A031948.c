/*
 * A031948  Numbers with exactly two distinct base-3 digits.
 * https://oeis.org/A031948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 111, u16 n <= 4308, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Brute‑force search for numbers with exactly two distinct base‑3 digits */
/* a(n) is the n-th positive integer whose base‑3 representation contains exactly two distinct digits. */

seq_t a(idx_t n)
{
    /* Start checking from 1; the sequence begins at 3. */
    seq_t val = 1;          /* current candidate */
    idx_t count = 0;        /* how many numbers found so far */

    while (count < n) {
        /* Determine which digits appear in 'val' in base 3. */
        seq_t tmp = val;
        unsigned mask = 0; /* bitmask for digits 0,1,2 */

        if (tmp == (seq_t)0) {          /* only occurs for val=0, which we ignore */
            mask = 1u;                  /* digit 0 */
        } else {
            while (tmp > (seq_t)0) {
                unsigned d = (unsigned)(tmp % (seq_t)3);
                mask |= (1u << d);
                tmp /= (seq_t)3;
            }
        }

        /* Count how many distinct digits were used. */
        unsigned bitCount =
            ((mask & 1u) >> 0) +
            ((mask & 2u) >> 1) +
            ((mask & 4u) >> 2);

        if (bitCount == 2) {           /* exactly two distinct digits */
            ++count;
        }
        ++val;                          /* move to the next integer */
    }

    /* We incremented val one past the desired number. */
    return val - (seq_t)1;
}
