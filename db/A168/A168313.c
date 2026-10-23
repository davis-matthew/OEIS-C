/*
 * A168313  Triangle read by rows, retain 1's as rightmost diagonal of A101688
 *          and replace all other 1's with 2's.
 * https://oeis.org/A168313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Read the triangle row‑by‑row with rows of length r. * Each row
 *            consists of floor(r/2) zeros, then (ceil(r/2)-1) twos, * and
 *            finally a single 1 at the rightmost position. * For a given n,
 *            find the row r such that r(r+1)/2 ≥ n. * Let pos = n − r(r−1)/2
 *            − 1 be the zero‑based column in that row. * Re...
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle non‑positive indices gracefully. */
    if (n == 0) return 0;

    idx_t r = 1;
    idx_t sum = 1;          /* sum of 1..r */
    while (sum < n) {      /* find minimal r with sum ≥ n */
        ++r;
        sum += r;
    }

    idx_t prev = sum - r;          /* sum of 1..(r-1) */
    idx_t pos  = n - prev - 1;     /* zero‑based column in row r */
    idx_t zeros = r / 2;           /* floor(r/2) leading zeros */

    if (pos < zeros) return 0;     /* leading zeros */
    if (pos == r - 1)   return 1;  /* rightmost 1 */
    return 2;                        /* other ones become 2 */
}
