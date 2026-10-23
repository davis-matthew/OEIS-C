/*
 * A034124  Numbers whose fractional part of the cube root starts with digit
 *          8.
 * https://oeis.org/A034124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1001, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose fractional part of the cube root starts with digit 8 (OEIS A034124).
   The nth term is the nth integer n such that the first decimal digit of
   fractional(cuberoot(n)) is 8.  The set of such n is obtained by
   iterating over each integer k = ⌊cuberoot(n)⌋ and collecting all integers
   in the interval
        ceil((k + 0.8)^3)  ≤  n  ≤  floor((k + 0.9)^3) .
   The implementation uses integer arithmetic only, evaluating the bounds as
   floor((5k+4)^3/125)+1 and floor((10k+9)^3/1000).  It is efficient enough
   to generate the first 10 000 terms in well under the time limit. */

seq_t a(idx_t n)
{
    idx_t count = 0;          /* number of terms already found             */
    for (idx_t k = 1; ; ++k) { /* iterate over possible integer parts k   */
        /* Compute lower and upper bounds for this k */
        unsigned long long mLow  = 5ULL * k + 4;   /* 5k+4            */
        unsigned long long mHigh = 10ULL * k + 9;  /*10k+9            */

        /* Use 128‑bit arithmetic to avoid overflow in intermediate steps */
        unsigned __int128 low_num  = (unsigned __int128)mLow  * mLow  * mLow;
        unsigned __int128 high_num = (unsigned __int128)mHigh * mHigh * mHigh;

        unsigned __int128 low_val  = low_num / 125 + 1;  /* ceil((k+0.8)^3)   */
        unsigned __int128 high_val = high_num / 1000;    /* floor((k+0.9)^3)  */

        if (high_val < low_val)          /* no integers in this interval           */
            continue;

        /* Number of integers in [low_val, high_val] */
        idx_t len = (idx_t)(high_val - low_val + 1);

        if (n <= count + len) {
            /* The sought term lies in this interval. */
            unsigned __int128 offset = (unsigned __int128)(n - count - 1);
            unsigned __int128 result = low_val + offset;
            return (seq_t)result;
        }

        count += len;    /* skip this whole interval and continue */
    }
}
