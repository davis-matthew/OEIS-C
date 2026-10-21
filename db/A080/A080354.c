/*
 * A080354  First differences of A080353.
 * https://oeis.org/A080354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A080354: first differences of A080353.  
   The sequence is a concatenation of runs:
     - first run: two occurrences of 2  (= 2^2),
     - for k = 1,2,3,... : pattern [2, 1^k] repeated 3 times.
   Thus for a given index n, find the run k it falls in and its offset
   within that run.  If the offset modulo (k+1) is zero the term is 2,
   otherwise the term is 1. */

seq_t a(idx_t n)
{
    /* Handle the very first two terms directly. */
    if (n < 2)
        return (seq_t)2;          /* a(0) = a(1) = 2 */

    idx_t k       = 1;            /* start with pattern length 2 repeated 3 times */
    idx_t total   = 2;            /* cumulative length after processing the first run */

    /* Find the run that contains index n. */
    while (1) {
        idx_t run_len = 3 * (k + 1);   /* each non‑zero run contributes 3*(k+1) terms */
        if (n < total + run_len)
            break;
        total += run_len;
        ++k;
    }

    /* Position within the current run. */
    idx_t offset   = n - total;          /* 0‑based offset inside this run   */
    idx_t base_len = k + 1;              /* length of the inner pattern     */

    return (offset % base_len == 0) ? (seq_t)2 : (seq_t)1;
}
