/*
 * A073089  a(n) = (1/2)*(4n - 3 - Sum_{k=1..n} A007400(k)).
 * https://oeis.org/A073089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9912, u16 n <= 9912, u32 n <= 9912, u64 n <= 9912,
 *            u128 n <= 9912, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) defined by the recurrence given in OEIS A073089:
 *   a(1) = 0
 *   a(4n)   = 1      (n >= 0)
 *   a(4n+2) = 0
 *   a(8n+3) = 1
 *   a(8n+7) = 0
 *   a(16n+5)= 1
 *   a(16n+13)=0
 *   a(8n+1)=a(4n+1)       (n >= 1)
 *
 * This implementation reduces n using the recurrence until it reaches a
 * base case, returning the appropriate 0 or 1 value.
 */

#include <stdint.h> /* for uint64_t, not strictly required but harmless */

seq_t a(idx_t n)
{
    /* base case for the OEIS offset */
    if (n == 1U)
        return (seq_t)0;

    /* iterate until a base pattern is found */
    for (;;) {
        /* one or two-digit checks first for speed */
        if ((n & 3ULL) == 0)   /* n % 4 == 0   → 1 */
            return (seq_t)1;
        if ((n & 3ULL) == 2)   /* n % 4 == 2   → 0 */
            return (seq_t)0;

        unsigned mod8  = (unsigned)(n & 7ULL);
        if (mod8 == 3)         /* 8n+3     → 1 */
            return (seq_t)1;
        if (mod8 == 7)         /* 8n+7     → 0 */
            return (seq_t)0;

        unsigned mod16 = (unsigned)(n & 15ULL);
        if (mod16 == 5)        /* 16n+5    → 1 */
            return (seq_t)1;
        if (mod16 == 13)       /* 16n+13   → 0 */
            return (seq_t)0;

        /* Remaining odd numbers are of the form 8n+1 (n >= 1) */
        /* a(8n+1) = a(4n+1)  → reduce by a factor of 2 and add 1 */
        n = ((n - 1ULL) >> 1) + 1ULL;  /* (n-1)/2 + 1, safe for unsigned */
    }
}
