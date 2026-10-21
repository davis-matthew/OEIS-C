/*
 * A057554  Lexicographic ordering of MxM, where M={0,1,2,...}.
 * https://oeis.org/A057554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8878, u16 n <= 8878, u32 n <= 8878, u64 n <= 8878,
 *            u128 n <= 8878, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS A057554: lexicographic ordering of M×M
   where pairs (i,j) are listed by increasing sum s = i + j and
   within each sum by increasing i.  The sequence consists of the
   flattened coordinates, so a(1)=0 from (0,0), a(2)=0, a(3)=0
   from (0,1), etc.  This implementation determines the sum
   s by incrementing until (s+1)(s+2) ≥ n, then computes the
   offset within that anti‑diagonal and returns either the
   first or second coordinate of the corresponding pair. */
seq_t a(idx_t n)
{
    /* Find the smallest s such that (s+1)*(s+2) >= n. */
    idx_t s = 0;
    while (((unsigned __int128)(s + 1) * (s + 2)) < n)
        ++s;

    /* Number of terms coming from sums < s. */
    idx_t prev = s * (s + 1);
    /* n is 1–based, offset is 1–based within the current sum. */
    idx_t offset = n - prev;          /* 1 … 2*(s+1) */
    idx_t pair   = (offset - 1) >> 1; /* pair index: 0 … s   */

    if (offset & 1U)     /* odd offset → first coordinate */
        return (seq_t)pair;
    else                 /* even offset → second coordinate */
        return (seq_t)(s - pair);
}
