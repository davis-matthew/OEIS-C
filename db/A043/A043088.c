/*
 * A043088  (s(n)+1)/10, where s(n)=n-th base 10 palindrome that starts with
 *          9.
 * https://oeis.org/A043088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 22, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   pow10, reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th element of sequence A043088 by enumerating
   base‑10 palindromes that start with the digit 9.  
   For each n it finds the corresponding palindrome s(n) and returns
   (s(n)+1)/10. */

#include "../utils/pow10.h"            /* seq_t pow10(idx_t e); */
#include "../utils/reverse_decimal.h"  /* seq_t reverse_decimal(seq_t v); */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset 1 is the first valid term */

    idx_t remaining = n;

    /* length = 1 : palindrome 9 */
    if (remaining == 1) return 1;    /* (9+1)/10 == 1 */
    --remaining;

    /* length = 2 : palindrome 99 */
    if (remaining == 1) return 10;   /* (99+1)/10 == 10 */
    --remaining;

    /* lengths >= 3 */
    for (idx_t L = 3; ; ++L)
    {
        idx_t exp = (L % 2) ? (L / 2) : ((L / 2) - 1); /* 10^exp palindromes of this length */

        /* compute 10^exp but stop if it exceeds remaining */
        idx_t count = 1;
        for (idx_t i = 0; i < exp; ++i)
        {
            if (count > remaining / 10) { count = remaining + 1; break; }
            count *= 10;
        }

        if (remaining > count)
        {
            remaining -= count;
            continue;
        }

        /* we are inside this length */
        idx_t offset = remaining - 1;            /* 0‑based position */

        idx_t k = L / 2;                         /* half‑length */
        seq_t powk = pow10(k);                   /* 10^k as seq_t */

        seq_t base;
        if (L % 2)                               /* odd length: 9*10^k */
            base = 9 * powk;
        else                                      /* even length: 9*10^(k-1) */
            base = 9 * pow10(k - 1);

        seq_t firstHalf = base + (seq_t)offset;

        seq_t palindrome;
        if (L % 2)                               /* odd */
        {
            seq_t fh_no_last = firstHalf / 10;
            seq_t rev = reverse_decimal(fh_no_last);
            palindrome = firstHalf * powk + rev;
        }
        else                                     /* even */
        {
            seq_t rev = reverse_decimal(firstHalf);
            palindrome = firstHalf * powk + rev;
        }

        return (palindrome + 1) / 10;
    }
}
