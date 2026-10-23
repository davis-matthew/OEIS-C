/*
 * A032815  Numbers whose set of base-15 digits is {2,3}.
 * https://oeis.org/A032815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 30, u32 n <= 510, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose base‑15 digits are only 2 or 3.
   The sequence is enumerated in increasing order.  */
seq_t a(idx_t n)
{
    /* n is 1‑based; behaviour for n==0 is undefined. */
    if (n == 0)
        return 0;          /* placeholder – should never happen */

    /* Find the length L of the base‑15 representation:
       cumulative numbers with lengths <= L is 2^(L+1) - 2.
       Find the smallest L with 2^(L+1) - 2 >= n. */
    idx_t L = 0;
    idx_t pow = 2;            /* 2^(L+1) */
    idx_t target = n + 2;     /* need pow >= target */
    while (pow < target) {
        pow <<= 1;            /* multiply by 2 */
        ++L;
    }

    /* Numbers with lengths < L amount to 2^L - 2. */
    idx_t cumulative_prev = ((idx_t)1 << L) - 2;
    /* Position (1‑based) within the block of length L numbers. */
    idx_t pos = n - cumulative_prev;   /* 1 <= pos <= 2^L */
    /* Zero‑based index for bits mapping 0→2 and 1→3. */
    idx_t bits = pos - 1;

    /* Build the value in base 15, most significant digit first. */
    seq_t val = 0;
    for (idx_t i = L; i > 0; --i) {
        idx_t shift = i - 1;
        seq_t digit = (seq_t)(2 + ((bits >> shift) & 1));
        val = val * (seq_t)15 + digit;
    }
    return val;
}
