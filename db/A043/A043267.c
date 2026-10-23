/*
 * A043267  Sum of the digits of the n-th base 8 palindrome.
 * https://oeis.org/A043267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of base‑8 digits of the n‑th base‑8 palindrome
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first 8 single‑digit palindromes */
    if (n <= 8)
        return (seq_t)(n - 1);      /* 0,1,2,…,7 */

    /* index of the palindrome among lengths >= 2 (1‑based) */
    idx_t idx = n - 8;

    /* find the length L such that the nth palindrome lies in
       the group of palindromes of length L */
    idx_t L = 2;
    while (1) {
        idx_t k   = (L + 1) / 2;            /* ceil(L/2) */
        idx_t exp = k - 1;                  /* number of free digits
                                               after the leading one */
        idx_t pow8 = 1;
        for (idx_t i = 0; i < exp; ++i)
            pow8 *= 8;                      /* 8^exp, safe for n ≤ 10000 */

        idx_t cnt = 7 * pow8;               /* palindromes of this length */
        if (idx <= cnt)
            break;                          /* found the correct length */

        idx -= cnt;                         /* skip this group */
        ++L;
    }

    /* build the first half (prefix) of the palindrome */
    idx_t k       = (L + 1) / 2;              /* prefix length            */
    idx_t exp     = k - 1;
    idx_t pow8    = 1;
    for (idx_t i = 0; i < exp; ++i)
        pow8 *= 8;                          /* 8^exp again */

    /* leading digit and the remaining digits */
    idx_t leadIndex   = (idx - 1) / pow8;     /* 0 … 6 */
    int    leadDigit  = 1 + (int)leadIndex;   /* 1 … 7 */

    idx_t suffixIndex = (idx - 1) % pow8;     /* (k‑1)‑digit number in base 8 */

    int prefix[64];                          /* enough for all needed L */
    prefix[0] = leadDigit;

    /* extract the (k‑1) remaining digits, most‑significant first */
    int rem = k - 1;
    idx_t tmp = suffixIndex;
    for (int p = rem; p >= 1; --p) {
        prefix[p] = tmp % 8;
        tmp /= 8;
    }

    /* compute the digit sum of the full palindrome */
    seq_t sum = 0;
    for (idx_t i = 0; i < k; ++i)
        sum += (seq_t)prefix[i];

    if (L % 2 == 0) {                       /* even length, mirror all */
        for (idx_t i = 0; i < k; ++i)
            sum += (seq_t)prefix[i];
    } else {                                 /* odd length, mirror except middle */
        for (idx_t i = 0; i < k - 1; ++i)
            sum += (seq_t)prefix[i];
    }

    return sum;
}
