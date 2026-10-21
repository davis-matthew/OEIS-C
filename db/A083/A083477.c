/*
 * A083477  Smallest palindrome > 1 and == 1 (mod n-th palindrome).
 * https://oeis.org/A083477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest palindrome > 1 such that a(n) ≡ 1 (mod
 *            A002113(n)). Search is performed by generating palindromes in
 *            increasing order until the congruence holds
 * Range:     u8 n <= 12, u16 n <= 42, u32 n <= 137, u64 n <= 208,
 *            u128 n <= 208, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* build 10^exp as a seq_t value */
static seq_t pow10_int(unsigned exp)
{
    seq_t r = 1;
    for (unsigned i = 0; i < exp; ++i)
        r *= (seq_t)10;
    return r;
}

/* reverse the lowest `digits` decimal digits of `x` */
static seq_t reverse_digits(seq_t x, unsigned digits)
{
    seq_t rev = 0;
    while (digits-- > 0)
    {
        rev = rev * (seq_t)10 + (x % (seq_t)10);
        x   = x / (seq_t)10;
    }
    return rev;
}

/* build a palindrome of total length `len` from the left part `left`.
   `leftLen` is the number of digits in `left` ( = (len+1)/2 ). */
static seq_t build_pal_from_left(seq_t left, unsigned len, unsigned leftLen)
{
    unsigned rightDigits = len - leftLen;
    seq_t factor = pow10_int(rightDigits);   /* 10^(len-leftLen)   */

    if (len % 2 == 0)      /* even length: use all digits of left */
    {
        seq_t rev = reverse_digits(left, leftLen);
        return left * factor + rev;
    }
    else                   /* odd length: skip the middle digit of left */
    {
        seq_t rev = reverse_digits(left / (seq_t)10, rightDigits);
        return left * factor + rev;
    }
}

/* return the n‑th decimal palindrome (A002113).  n >= 1. */
static seq_t nth_palindrome(idx_t n)
{
    seq_t nseq = (seq_t)n;
    unsigned len = 1;
    while (1)
    {
        unsigned leftLen = (len + 1) / 2;
        seq_t count = 9 * pow10_int(leftLen - 1);      /* number of palindromes of this length */
        if (nseq <= count)
        {
            seq_t offset = nseq - (seq_t)1;
            seq_t left = pow10_int(leftLen - 1) + offset;
            return build_pal_from_left(left, len, leftLen);
        }
        nseq -= count;
        ++len;
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    seq_t p = nth_palindrome(n);               /* p = A002113(n) */
    if (p == (seq_t)1)                         /* special case n = 1 */
        return (seq_t)2;                       /* smallest palindrome > 1 */

    for (idx_t k = 1; ; ++k)
    {
        seq_t pal = nth_palindrome(k);
        if (pal == (seq_t)1) continue;         /* skip the trivial palindrome 1 */
        if (pal % p == (seq_t)1)
            return pal;
    }
}
