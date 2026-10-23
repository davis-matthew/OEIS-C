/*
 * A138709  n-th run has length n-th positive Fibonacci numbers, with digits 0
 *          and 1 only, starting with 1.
 * https://oeis.org/A138709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A138709: 0/1 runs of lengths given by successive Fibonacci numbers
   starting with 1.  a(n) is 1 if the n-th position lies in an odd‑indexed
   Fibonacci run, 0 otherwise. This implementation computes a(n) by
   generating Fibonacci lengths until the cumulative sum exceeds n. */

seq_t a(idx_t n)
{
    /* Fibonacci generation: aFib=F_{k-2}, bFib=F_{k-1}. */
    idx_t aFib = 1, bFib = 1;   /* F1 = 1, F2 = 1   */
    idx_t sum  = 0;             /* cumulative run length processed   */
    idx_t k    = 1;             /* current run number (1‑based)       */

    while (1) {
        idx_t runLen;

        if (k == 1 || k == 2)
            runLen = 1;
        else {
            runLen = aFib + bFib;
            /* advance Fibonacci pair for next iteration */
            aFib = bFib;
            bFib = runLen;
        }

        sum += runLen;
        if (n <= sum)
            return (k & 1) ? (seq_t)1 : (seq_t)0;

        k++;
    }
}
