/*
 * A030149  Even palindromes in which parity of digits alternates.
 * https://oeis.org/A030149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 88, u32 n <= 3125, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A030149 – Even palindromes with alternating parity.
   Enumerated in ascending numeric order.  The function computes the
   n‑th term (n ≥ 1) without any lookup table. */
seq_t a(idx_t n)
{
    /* The first five terms are the single‑digit even numbers. */
    if (n <= 5)
        return (seq_t)((n - 1) * 2);

    /* Remaining index after the first five terms.  (1‑based) */
    idx_t remaining = n - 5;

    /* Find the block (length) that contains the requested term.
       Length  = 2*halfLen-1,  halfLen starts at 2 (length 3).  */
    idx_t halfLen = 2;           /* number of independent digits */
    idx_t pow5    = 5;           /* 5^(halfLen-1) for block size */
    idx_t block   = 4 * pow5;    /* 4 * pow5^(halfLen-1) */

    while (remaining > block) {
        remaining -= block;     /* skip this block */
        halfLen++;              /* move to next odd length */
        pow5 *= 5;              /* multiply by 5 for the new power */
        block = 4 * pow5;
    }

    /* Prepare the digit choices for each independent position.
       position 0 : even non‑zero digits 2,4,6,8  -> 4 choices
       all other positions : 5 choices
    */
    unsigned char bases[32];
    bases[0] = 4;                          /* first digit */
    for (idx_t i = 1; i < halfLen; ++i)
        bases[i] = 5;

    /* Mixed‑radix factors for decoding. */
    idx_t factor[32];
    factor[halfLen - 1] = 1;
    for (idx_t i = halfLen - 2; i < halfLen; --i) { /* unsigned wrap‑around guard */
        factor[i] = factor[i + 1] * (idx_t)bases[i + 1];
    }

    /* zero‑based offset within the block. */
    idx_t pos = remaining - 1;
    uint8_t digits[32];

    for (idx_t i = 0; i < halfLen; ++i) {
        idx_t d = pos / factor[i];
        pos %= factor[i];

        if (i == 0) {
            /* first digit: 2,4,6,8 */
            digits[i] = (uint8_t)(2 + 2 * d);
        } else if ((i & 1) == 0) {
            /* even positions > 0: 0,2,4,6,8 */
            digits[i] = (uint8_t)(2 * d);
        } else {
            /* odd positions: 1,3,5,7,9 */
            digits[i] = (uint8_t)(1 + 2 * d);
        }
    }

    /* Build the full palindrome. */
    seq_t val = 0;
    for (idx_t i = 0; i < halfLen; ++i)
        val = val * 10 + digits[i];
    for (idx_t i = halfLen - 2; i < halfLen; --i) {          /* unsigned wrap‑around guard */
        val = val * 10 + digits[i];
    }

    return val;
}
