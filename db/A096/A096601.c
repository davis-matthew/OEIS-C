/*
 * A096601  Triangle (read by rows) where the number of row entries increases
 *          by steps of 2 and the entries are stacked in a rectangular
 *          fashion. The end entries are set to 1. Rest of entries are set to
 *          the sum of the entry directly above in the previous row plus the
 *          entry (if present) in the previous row th...
 * https://oeis.org/A096601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A096601: triangular array defined recursively:
   T(1,0)=1.
   For r≥2, T(r,0)=T(r,2r-2)=1.
   For 0<c<2r-2:
       if r is odd: T(r,c)=T(r-1,c-1)+(c<2(r-1)-1?T(r-1,c):0)
       if r is even: T(r,c)=T(r-1,c-1)+(c>=2?T(r-1,c-2):0)
   The sequence a(n) is T(r,c) read row by row, offset 1.
*/
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    /* locate the row number r such that r^2 ≥ n */
    idx_t r = 1;
    while (r * r < n) r++;
    /* buffers for consecutive rows – maximum row length for n≤100 000 is < 200 */
    seq_t prev[200];
    seq_t cur[200];
    idx_t prev_len = 1;
    prev[0] = (seq_t)1;
    if (r == 1) return prev[0];
    for (idx_t row = 2; row <= r; row++) {
        idx_t cur_len = 2 * row - 1;
        cur[0] = (seq_t)1;
        cur[cur_len - 1] = (seq_t)1;
        for (idx_t j = 1; j < cur_len - 1; j++) {
            seq_t sum = prev[j - 1];
            if (row & 1) {                 /* row odd – add right neighbour if present */
                if (j < prev_len) sum += prev[j];
            } else {                      /* row even – add left neighbour if present */
                if (j >= 2) sum += prev[j - 2];
            }
            cur[j] = sum;
        }
        /* prepare for next iteration */
        for (idx_t j = 0; j < cur_len; j++) prev[j] = cur[j];
        prev_len = cur_len;
    }
    /* index of desired element within the found row (0‑based) */
    idx_t offset = n - (r - 1) * (r - 1) - 1;
    return prev[offset];
}
