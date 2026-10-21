/*
 * A050782  Smallest positive multiplier m such that m*n is palindromic (or
 *          zero if no such m exists).
 * https://oeis.org/A050782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, nice
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 80, u32 n <= 8180, u64 n <= 8990,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A050782: smallest multiplier m such that m*n is a decimal palindrome.
   If no such m exists (e.g. when n is a multiple of 10) the function returns 0.
   The algorithm generates palindromes in increasing numeric order: 1‑digit palindromes
   are checked first, then 2‑digit, 3‑digit, etc.  For each length the first half
   of the palindrome is enumerated from the smallest possible value upward,
   guaranteeing that the first palindrome found is the minimal one.  The result
   m is obtained by integer division of the palindrome by n.  The implementation
   is generic for arbitrary‑precision or fixed‑size integer types that support
   the usual arithmetic and modulo operators. */

/* --- small helper: compute 10^e as seq_t ---------------------------------- */
static seq_t pow10(idx_t e)
{
    seq_t r = (seq_t)1;
    for (idx_t i = 0; i < e; ++i)
        r *= (seq_t)10;
    return r;
}

/* --- build a decimal palindrome -------------------------------
   half   : integer representing the first ceil(len/2) digits
   len    : total number of digits of the resulting palindrome
   Returns the palindrome as a seq_t value.
   Method: decompose 'half' into a list of its digits (least‑significant first),
          then rebuild the palindrome:
          (i) write the digits of 'half' in most‑significant‑first order
          (ii) append the reverse of the digits, omitting the middle digit
               when len is odd.  The middle digit corresponds to the least‑significant
               digit of 'half' (index 0).
--------------------------------------------------------------------- */
static seq_t build_pal(seq_t half, int len)
{
    int k = (len + 1) / 2;             /* number of digits in 'half'          */
    uint8_t digs[64];                  /* sufficient for the task             */

    /* extract digits of 'half' (least‑significant first) */
    seq_t tmp = half;
    for (int i = 0; i < k; ++i) {
        digs[i] = (uint8_t)(tmp % (seq_t)10);
        tmp   /= (seq_t)10;
    }

    seq_t pal = (seq_t)0;

    /* (i) first half — most‑significant‑first order */
    for (int i = 0; i < k; ++i)
        pal = pal * (seq_t)10 + (seq_t)digs[k - 1 - i];

    /* (ii) mirrored part */
    int start = (len % 2) ? 1 : 0;      /* start index in digs for mirroring  */
    for (int i = start; i < k; ++i)
        pal = pal * (seq_t)10 + (seq_t)digs[i];

    return pal;
}

/* --- public routine ------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)           /* defined as 0 by the OEIS entry */
        return (seq_t)0;

    /* A palindrome cannot end with 0; if n is a multiple of 10 no such
       multiplier exists. */
    if (n % (idx_t)10 == (idx_t)0)
        return (seq_t)0;

    seq_t sn = (seq_t)n;

    for (int len = 1; /* infinite */ ; ++len) {
        int k = (len + 1) / 2;
        seq_t lower = pow10((idx_t)(k - 1));      /* smallest k‑digit number */
        seq_t upper = pow10((idx_t)k) - (seq_t)1; /* largest k‑digit number  */

        for (seq_t h = lower; h <= upper; ++h) {
            seq_t pal = build_pal(h, len);
            if (pal % sn == (seq_t)0)
                return pal / sn;                 /* minimal multiplier m */
        }
    }
}
