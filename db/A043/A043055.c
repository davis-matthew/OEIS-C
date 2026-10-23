/*
 * A043055  a(n)=(s(n)+4)/6, where s(n)=n-th base 6 palindrome that starts
 *          with 2.
 * https://oeis.org/A043055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 163, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = (s(n)+4)/6 where s(n) is the n-th base‑6 palindrome
   that starts with the digit 2.  The palindromes are enumerated in
   increasing base‑10 value. */
seq_t a(idx_t n)
{
    /* pre‑compute powers of 6; enough for all n up to 10000 */
    idx_t pow6[20];
    pow6[0] = 1;
    for (idx_t i = 1; i < 20; ++i) {
        pow6[i] = pow6[i-1] * (idx_t)6;
    }

    /* find the palindrome length containing the n‑th element */
    idx_t target = n;           /* remaining index we need to find */
    idx_t len    = 1;           /* current length in base‑6 digits */

    while (1) {
        idx_t half = (len + 1) / 2;          /* number of free digits */
        idx_t count = pow6[half - 1];        /* palindromes of this length */
        if (target <= count) break;          /* found the length */
        target -= count;
        ++len;
    }

    /* Now we need the (target‑1)-th such palindrome (0‑based). */
    idx_t k = target - 1;

    /* build the half digits: first digit fixed to 2 */
    unsigned char halfDigits[20];
    halfDigits[0] = 2;
    idx_t half = (len + 1) / 2;
    for (idx_t i = 1; i < half; ++i) halfDigits[i] = 0;

    /* assign the varying digits from k (base‑6 representation) */
    idx_t tmp = k;
    for (idx_t pos = half - 1; pos >= 1; --pos) {
        halfDigits[pos] = tmp % 6;
        tmp /= 6;
    }

    /* construct the full palindrome digits */
    unsigned char digits[20];
    for (idx_t i = 0; i < len; ++i) {
        if (i < half) digits[i] = halfDigits[i];
        else digits[i] = digits[len - 1 - i];
    }

    /* convert to decimal (seq_t).  s is the desired base‑6 palindrome. */
    seq_t s = 0;
    for (idx_t i = 0; i < len; ++i) {
        s = s * (seq_t)6 + (seq_t)digits[i];
    }

    /* return (s + 4) / 6 */
    seq_t result = (s + (seq_t)4) / (seq_t)6;
    return result;
}
