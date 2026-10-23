/*
 * A193434  6*n/5 = (n written backwards), n > 0.
 * https://oeis.org/A193434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 45 × the n‑th palindrome consisting of only the digits 0
 *            and 1, with first digit 1 (i.e. the n‑th term of OEIS A057148)
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 30, u64 n <= 1022,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 45; a(2) = 495; … */

seq_t a(idx_t n)
{
    if (n == 0)                /* offset starts at 1 – not used */
        return (seq_t)0;

    /* Find the length L of the palindrome that contains the n‑th term. */
    idx_t rem   = n;            /* remaining steps to reach target */
    unsigned L  = 1;            /* current length */
    while (1) {
        /* Number of palindromes of length L:
           2^(ceil(L/2)-1)                                 */
        unsigned half  = (L + 1) / 2;          /* ceil(L/2)          */
        idx_t   count  = (idx_t)1 << (half - 1); /* 2^(half-1)        */
        if (rem <= count)           /* target in this block */
            break;
        rem -= count;               /* skip this block    */
        ++L;                         /* next length        */
    }

    /* Position inside the block (0‑based). */
    idx_t   posWithin   = rem - 1;           /* 0 … 2^(half-1)-1   */
    unsigned half       = (L + 1) / 2;       /* number of digits in the first half */

    /* Construct the palindrome digit by digit. */
    seq_t  val = (seq_t)0;
    for (unsigned p = 0; p < L; ++p) {
        unsigned mir = p < L - 1 - p ? p : L - 1 - p; /* mirrored position */
        unsigned digit;
        if (mir == 0)                                 /* first digit always 1 */
            digit = 1;
        else
            digit = (unsigned)((posWithin >> (half - 1 - mir)) & 1);
        val = val * (seq_t)10 + (seq_t)digit;
    }

    /* Multiply by 45 to obtain the A193434 term. */
    return (seq_t)((seq_t)45 * val);
}
