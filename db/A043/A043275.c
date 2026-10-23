/*
 * A043275  Sum of the digits of the n-th base 16 palindrome.
 * https://oeis.org/A043275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of hexadecimal digits of nth base‑16 palindrome
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (offset 1)                                                */
/* This implementation builds the n‑th palindrome directly from its
   position in the sequence of base‑16 palindromes, then sums its
   hex digits without constructing the full decimal value.          */
seq_t a(idx_t n)
{
    /* Handle the first 16 palindromes 0 … F directly. */
    if (n <= 16)
        return (seq_t)(n - 1);

    /* Remaining index after the 0–F block. */
    idx_t rem = n - 16;

    /* Find the length L of the palindrome that contains this index. */
    idx_t L = 2;                     /* length cannot be 1 now */
    for (;; ++L)
    {
        idx_t halfLen = (L + 1) / 2; /* ceil(L/2) */
        idx_t exp = halfLen - 1;

        /* base = 16^(halfLen-1) */
        idx_t base = 1;
        for (idx_t i = 0; i < exp; ++i)
            base *= 16;

        idx_t count = 15 * base;     /* number of palindromes of this length */
        if (rem <= count)
            break;                   /* found the correct length */
        rem -= count;                /* skip this block */
    }

    /* Compute half length and base again for final calculation. */
    idx_t halfLen = (L + 1) / 2;
    idx_t exp = halfLen - 1;
    idx_t base = 1;
    for (idx_t i = 0; i < exp; ++i)
        base *= 16;

    /* k is zero‑based index within this block. */
    idx_t k = rem - 1;

    /* The half of the palindrome. */
    idx_t half = base + k;           /* first digit ∈ [1,15] */

    /* Sum of the digits of the half. */
    idx_t sumHalf = 0;
    idx_t tmp = half;
    while (tmp > 0)
    {
        sumHalf += tmp & 15;         /* tmp % 16 */
        tmp >>= 4;                   /* tmp / 16 */
    }

    /* Most significant digit of the entire palindrome. */
    idx_t firstDigit = half / base;   /* this is the leading hex digit */

    /* Compute total digit sum. */
    idx_t result;
    if ((L & 1) == 0)                /* even length: digits repeat twice */
        result = sumHalf * 2;
    else                              /* odd length: middle digit counted once */
        result = sumHalf + firstDigit;

    return (seq_t)result;
}
