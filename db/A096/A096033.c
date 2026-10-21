/*
 * A096033  Difference between leg and hypotenuse in primitive Pythagorean
 *          triangles.
 * https://oeis.org/A096033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 308, u32 n <= 9947, u64 n <= 9947,
 *            u128 n <= 9947, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A096033:
   a(n) is the nth term of the sorted union of odd squares and half of even squares,
   i.e. a(n) = nth smallest number in the set { (2k+1)^2 | k ≥ 0 } ∪ { 2m^2 | m ≥ 1 }.
   Example: 1, 2, 8, 9, 18, 25, 32, 49, …
   The function is O(n) by simple two‑pointer merge. */

seq_t a(idx_t n)
{
    /* indices for the two sequences:
       odd_k  – index in the odd‑square sequence (k starts at 0, value (2k+1)^2)
       even_m – index in the even‑half‑square sequence (m starts at 1, value 2*m^2) */
    idx_t odd_k  = 0;
    idx_t even_m = 1;

    /* current smallest values from each sequence */
    seq_t odd_val  = (seq_t)(2 * odd_k + 1) * (seq_t)(2 * odd_k + 1);  /* 1² = 1 */
    seq_t even_val = (seq_t)2 * (seq_t)even_m * (seq_t)even_m;          /* 2*1² = 2 */

    seq_t result = 0;
    idx_t count = 0;

    while (count < n) {
        if (odd_val <= even_val) {
            result = odd_val;
            odd_k++;
            odd_val = (seq_t)(2 * odd_k + 1);
            odd_val = odd_val * odd_val;
        } else {
            result = even_val;
            even_m++;
            even_val = (seq_t)2 * (seq_t)even_m * (seq_t)even_m;
        }
        count++;
    }
    return result;
}
